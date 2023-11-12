#include <pch.hpp>
#include <hacks/aimbot\autowall.hpp>
#include <hacks/aimbot/aimbot.hpp>
#include <globals/globals.hpp>
#include <offets/offsets.hpp>
#include <logger/logger.hpp>
#include <renderer/renderer.hpp>
#include <imgui/imgui_internal.h>
#include <math/math.hpp>
#include <debugoverlay/CDebugOverlayGameSystem.hpp>
#include <vars/vars.hpp>

using namespace AutoWall;
using namespace trace;
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

bool PreferBodyAim(int hitgroup, bool hasArmor)
{

    if (!hasArmor && hitgroup != HITGROUP::HITGROUP_HEAD) return true;

    switch (hitgroup)
    {
        case HITGROUP::HITGROUP_STOMACH:
            return true;
        case HITGROUP::HITGROUP_CHEST:
            return true;
    }
    return false;
}



void AutoWall::ScaleDamage(C_CSPlayerPawnBase* player, CCSWeaponBaseVData* weaponData, float& damage, int hitgroup, bool hasArmor)
{
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

bool AutoWall::CanHit(C_CSPlayerPawnBase* enemy, Vector localPlayerEyePosition, Vector targetPosition,
    CCSWeaponBaseVData* weaponInfo, float& damage)
{
    auto data = FireBulletData(localPlayerEyePosition);
    data.direction = (targetPosition - localPlayerEyePosition).Normalize();
    C_TraceFilter filter(0x1C300B, globals::localPlayerPawn, nullptr, 3);
    data.filter = filter;
    data.currentDamage = ( float )weaponInfo->m_nDamage();

    if (SimulateFireBullet(weaponInfo, data, enemy))
    {
        damage = data.currentDamage;
        return true;
    }
    return false;
}

bool RebuiltTraceToExit(Vector& end, C_GameTrace enterTrace, Vector start, Vector dir, C_GameTrace& exitTrace)
{

    float currentDistance = 0.0f;
    int firstContents {};

    while (currentDistance <= 90.f)
    {
        currentDistance += 4.0f;

        end = start + (dir * currentDistance);

        Vector traceEnd = end - (dir * 4.0f);

        int contents = 0;
        if (!firstContents)
            firstContents = offsets::GetContents(end, 0x1C3003, 7);;

        contents = offsets::GetContents(end, 0x1C3003, 7);

        if ((contents & 0x1C3001) == 0 || (contents & 2) != 0 && firstContents != contents)
        {
            C_Ray ray {};
            C_TraceFilter filter(0x1C3003, 0, 0, 0);

            offsets::TraceShape(&ray, end, traceEnd, &filter, &exitTrace);
            //CDebugOverlayGameSystem::Get()->AddTextOverlay(exitTrace.EndPos, 0.01f, 0.5f, IM_COL32(255, 255, 255, 255), "End");

            bool v27 = *(( BYTE* )&exitTrace + 0xB7);


            if (exitTrace.bStartSolid && exitTrace.nSurfaceFlags & 0x0080)
            {
                C_TraceFilter filter(0x1C3003, 0, exitTrace.pHitEntity, 0);
                offsets::TraceShape(&ray, end, start, &filter, &exitTrace);

                if (exitTrace.flFraction < 1.0f && !exitTrace.bStartSolid)
                {
                    end = exitTrace.vecEnd;
                    return true;
                }
                continue;
            }

            else if (exitTrace.flFraction < 1.f && !exitTrace.bStartSolid)
            {
                if (((exitTrace.nSurfaceFlags >> 7) & 1) && !((exitTrace.nSurfaceFlags >> 7) & 1))
                    continue;

                Vector normal = exitTrace.vecNormal;

                if (normal.x * dir.x + normal.y * dir.y + normal.z * dir.z <= 1.f)
                {
                    //CDebugOverlayGameSystem::Get()->AddTextOverlay(traceEnd, 0.001f, 2, IM_COL32(255, 255, 255, 255), "End Point");
                    end = end - (dir * 4.0f * exitTrace.flFraction);
                    return true;
                }
            }
        }
    }

    return false;
}

bool HealthPointsPlusOne(C_CSPlayerPawnBase* enemy, int damage)
{
    return g_Vars.m_MinimumDamage >= 100 && damage >= enemy->m_iHealth() + 1;
}

bool AutoWall::SimulateFireBullet(CCSWeaponBaseVData* weaponInfo, FireBulletData& data, C_CSPlayerPawnBase* enemy)
{
    TraceData traceData { };
    traceData.arrayPointer = &traceData.arr;
    Vector endPosition = data.direction * weaponInfo->m_flRange();

    C_TraceFilter filter(0x1C300B, globals::localPlayerPawn, 0, 3);
    offsets::CreateTrace(&traceData, data.src, endPosition, &filter, 4);

    if (traceData.numberOfUpdates <= 0) return false;

    WeaponData weaponData(weaponInfo->m_nDamage(), weaponInfo->m_flPenetration(), weaponInfo->m_flRange(), weaponInfo->m_flRangeModifier(), 4, false);

    for (int i { }; i < traceData.numberOfUpdates; i++)
    {
        auto* value = reinterpret_cast< UpdateValue* const >(reinterpret_cast< std::uintptr_t >(
            traceData.pointerUpdateValue) + i * sizeof(UpdateValue));

        C_GameTrace trace {};
        offsets::InitializeTrace(&trace);
        auto whatTheFuckIsThis = reinterpret_cast< void* >(
            reinterpret_cast< std::uintptr_t >(traceData.arr.data())
            + sizeof(TraceArrElements) * (value->handleIdx & 0x7FFF));

        offsets::GetTraceInfo(&traceData, &trace, 0.0f, whatTheFuckIsThis);


        if (trace.pHitEntity == enemy)
        {
            int hitBoxId = trace.GetHitboxId();
            bool hasArmor = PlayerHasArmor(enemy, hitBoxId);

            //previous damage = 101, hitbox == stomach, min damage = 70 should be true 
            ScaleDamage(enemy, weaponInfo, data.currentDamage, hitBoxId, hasArmor);

            if (g_Vars.m_PreferBodyAim && (PreferBodyAim(hitBoxId, hasArmor) && data.currentDamage >= g_Vars.m_MinimumDamage) ||
                (hitBoxId != HITGROUP::HITGROUP_HEAD && HealthPointsPlusOne(enemy, data.currentDamage)))
            {
                globals::aimbotData.hitScanPreference = HitScanPreference::PREFER;
                return true;
            }
            else if (HealthPointsPlusOne(enemy, data.currentDamage))
            {
                return true;
            }
            else if (data.currentDamage >= g_Vars.m_MinimumDamage)
            {
                return true;
            }
        }

        if (offsets::HandleBulletPenetration(&traceData, &weaponData, value, false))
            return false;

        data.currentDamage = weaponData.weaponDamage;
    }
    return false;
}

/*
bool AutoWall::SimulateFireBullet(CCSWeaponBaseVData* weaponData, FireBulletData& data, C_CSPlayerPawnBase* enemy)
{
    C_CSPlayerPawnBase* entityToSkip = nullptr;
    data.penetrateCount = 4;
    data.traceLength = 0.0f;
    data.currentDamage = weaponData->m_nDamage();

    while (data.penetrateCount > 0 && data.currentDamage >= 1.f)
    {
        data.traceLengthRemaining = weaponData->m_flRange() - data.traceLength;

        C_Ray ray;
        //Why is this needed?
        ray.UnkType = 0;
        ray.Start = ray.End = ray.Mins = ray.Maxs = Vector();

        Vector end = data.src + data.direction * data.traceLengthRemaining;

        offsets::TraceShape(&ray, data.src, end, &data.filter, &data.enterTrace);

        Vector newEndPoint = end + data.direction * 40.f;

        offsets::ClipTraceToPlayers(data.src, newEndPoint, &data.filter, &data.enterTrace, -60.0f, 0x42700000, -1);

        if (data.enterTrace.flFraction == 1.0f) break;

        data.traceLength += data.enterTrace.flFraction * data.traceLengthRemaining;
        data.currentDamage *= pow(weaponData->m_flRangeModifier(), data.traceLength * 0.002f);

        C_SurfaceData* enterSurfaceData = data.enterTrace.GetSurfaceData();

        if (data.traceLength > 3000 && weaponData->m_flPenetration() > 0.f || enterSurfaceData->PenetrationModifier < 0.1f)
            break;

        int hitgroup = data.enterTrace.GetHitGroup();

        if (hitgroup >= HITGROUP_HEAD && hitgroup <= HITGROUP_NECK)
        {
            ScaleDamage(enemy, weaponData, data.currentDamage, hitgroup);
            return true;
        }

        if (!RebuiltHandleBulletPenetration(weaponData, data) && !(offsets::GetContents(data.enterTrace.vecEnd, 0x1C3003, 0) & 0x1C3003))
            break;

    }
    return false;
}
                              */

