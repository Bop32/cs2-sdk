#include <pch.hpp>
#include <hacks/aimbot\autowall.hpp>
#include <hacks/aimbot/aimbot.hpp>
#include <bindings/trace.hpp>
#include <globals/globals.hpp>
#include <offets/offsets.hpp>
#include <logger/logger.hpp>
#include <renderer/renderer.hpp>
#include <imgui/imgui_internal.h>
#include <math/math.hpp>

bool PlayerHasArmor(C_CSPlayerPawnBase* player, int hitgroup)
{
    if (player->m_ArmorValue() <= 0) return false;

    switch (hitgroup)
    {
        case HITGROUP::HITGROUP_HEAD:
            return player->m_pItemServices()->m_bHasHelmet();
        case HITGROUP::HITGROUP_GENERIC:
        case HITGROUP::HITGROUP_CHEST:
        case HITGROUP::HITGROUP_STOMACH:
        case HITGROUP::HITGROUP_LEFTARM:
        case HITGROUP::HITGROUP_RIGHTARM:
            return true;
        default:
            return false;
    }

    return false;
}

void AutoWall::ScaleDamage(C_CSPlayerPawnBase* player, weapon::CCSWeaponBaseVData* weaponData, float& damage, int hitgroup)
{
    bool hasArmor = PlayerHasArmor(player, hitgroup);
    switch (hitgroup)
    {
        case HITGROUP::HITGROUP_HEAD:
            damage *= 4.f;
            break;
        case HITGROUP::HITGROUP_STOMACH:
            damage *= 1.25f;
            break;
        case HITGROUP::HITGROUP_LEFTLEG:
        case HITGROUP::HITGROUP_RIGHTLEG:
            damage *= 0.75f;
            break;
        default:
            break;
    }

    if (hasArmor)
    {
        float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weaponData->m_flArmorRatio() / 2.f;

        auto NewDamage = damage * armorRatio;

        if (((damage - (damage * armorRatio)) * (bonusValue * armorBonusRatio)) > player->m_ArmorValue())
            NewDamage = damage - (player->m_ArmorValue() / armorBonusRatio);

        damage = NewDamage;
    }
}

//Currently all this does is just check for the best hitbox not taking into account damage dropoff.
//Works for now as if you have multiple hitboxes selected it will go for the highest damage one
//Need to hook these two. LOOK AT TRACESHAPE FOR REFERENCE.
/*
    using SHandleBulletPenetration = bool(__fastcall*)(float&, int&, bool&, void*, FVector&, void*, float, float, bool, uint32, float, int&, FVector&, float, float, float&, uint32, void*);
    SHandleBulletPenetration HandleBulletPenetration = "E8 ? ? ? ? BA ? ? ? ? 48 8D 0D ? ? ? ? 0F B6 D8 E8 ? ? ? ? 48 85 C0 75 0B 48 8B 05 ? ? ? ? 48 8B 40 08 66 0F 6E 00" Follow JMP

    using SClipTraceToPlayers = void(__fastcall*)(const FVector&, const FVector&, STraceFilter*, SGameTrace*, float, float, float);
    SClipTraceToPlayers ClipTraceToPlayers = "E8 ? ? ? ? 80 BD ? ? ? ? ? 0F 85 ? ? ? ? F3 0F 10 85 ? ? ? ? 8B 85" Absoloute(1,0);

*/
bool AutoWall::CanHit(C_CSPlayerPawnBase* enemy, Vector localPlayerEyePosition, Vector targetPosition,
    weapon::CCSWeaponBaseVData* weaponInfo, float& damage)
{
    float maxRange = weaponInfo->m_flRange();
    float weaponDefaultDamage = weaponInfo->m_nDamage();
    float currentDistance = 0;
    int penetrationCount = 4;
    float weaponPenetration = weaponInfo->m_flPenetration();
    C_CSPlayerPawnBase* entityToSkip = nullptr;
    Vector direction = (targetPosition - localPlayerEyePosition).Normalize();

    auto drawList = CRenderer::GetBackgroundDrawList();

    while (penetrationCount > 0 && weaponDefaultDamage >= 1.f)
    {
        float LengthRemaining = maxRange - currentDistance;

        Vector end = localPlayerEyePosition + direction * LengthRemaining;


        trace::C_Ray ray;
        trace::C_GameTrace trace;
        trace::C_TraceFilter filter(0x1C1003, globals::localPlayerPawn, entityToSkip, 3);
        //Why is this needed?
        ray.UnkType = 0;
        ray.Start = ray.End = ray.Mins = ray.Maxs = Vector();
        filter.V4 |= 2;


        offsets::TraceShape(&ray, localPlayerEyePosition, end, &filter, &trace);

        Vector endPoint = end + direction * 40.f;

        auto val1 = (localPlayerEyePosition - end).Length();
        auto val2 = (trace.EndPos - localPlayerEyePosition).Length();

        auto val3 = 1.f / val1 * val2;

        auto val4 = (1.F / (localPlayerEyePosition - end).Length()) * (trace.EndPos - localPlayerEyePosition).Length();
        offsets::ClipTraceToPlayers(localPlayerEyePosition, endPoint, &filter, &trace, 0.f, 60.f, (1.F / (localPlayerEyePosition - end).Length()) * (trace.EndPos - localPlayerEyePosition).Length());

        C_CSPlayerPawnBase* hitEntity = trace.HitEntity;



        if (!hitEntity) break;

        if (hitEntity->IsPlayerPawn())
        {
            entityToSkip = hitEntity;
        }

        if (trace.Fraction == 1.f) break;

        trace::C_SurfaceData* Surface = trace.GetSurfaceData();
        int material = Surface->Material;
        bool bHitGrate = (trace.Contents & 0x2000) != 0;

        int hitgroup = trace.GetHitGroup();

        if (hitgroup >= HITGROUP_HEAD && hitgroup <= HITGROUP_NECK && hitEntity == enemy)
        {
            ScaleDamage(enemy, weaponInfo, weaponDefaultDamage, hitgroup);
            damage = weaponDefaultDamage;

            return true;
        }

        currentDistance += trace.Fraction * maxRange;

        weaponDefaultDamage *= pow(weaponInfo->m_flRangeModifier(), currentDistance * 0.002f);

        float PenetrationMod = Surface->PenetrationModifier;
        if (weaponDefaultDamage > 3000.f && weaponPenetration > 0.f || PenetrationMod < 0.1f)
            penetrationCount = 0;

        /*
        bool bHBP = offsets::HandleBulletPenetration(
            weaponInfo->m_flPenetration(), // a1
            material,                      // a2
            bHitGrate,                     // a3
            &trace,                        // a4
            direction,                     // a5
            trace.Surface,                 // a6
            PenetrationMod,                // a7
            Surface->DamageModifier,       // a8
            false,                         // a9
            2,                             // a10
            weaponPenetration,             // a11
            penetrationCount,              // a12
            localPlayerEyePosition,        // a13
            maxRange,                      // a14
            currentDistance,               // a15
            CGameEntitySystem::GetLocalPlayerController()->m_iTeamNum(),     // a16
            CGameEntitySystem::GetLocalPlayerController()                    // a17
        );

        if (bHBP)
            break;
        */
        return false;
    }
    return false;
}

