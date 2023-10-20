#include <pch.hpp>
#include <hacks/aimbot\autowall.hpp>
#include <hacks/aimbot/aimbot.hpp>
#include <globals/globals.hpp>
#include <offets/offsets.hpp>
#include <logger/logger.hpp>
#include <renderer/renderer.hpp>
#include <imgui/imgui_internal.h>
#include <math/math.hpp>
using namespace AutoWall;

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

void AutoWall::ScaleDamage(C_CSPlayerPawnBase* player, CCSWeaponBaseVData* weaponData, float& damage, int hitgroup)
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

bool AutoWall::CanHit(C_CSPlayerPawnBase* enemy, Vector localPlayerEyePosition, Vector targetPosition,
    CCSWeaponBaseVData* weaponInfo, float& damage)
{
    auto data = FireBulletData(localPlayerEyePosition);
    data.direction = (targetPosition - localPlayerEyePosition).Normalize();
    trace::C_TraceFilter filter(0x1C300B, globals::localPlayerPawn, nullptr, 3);
    data.filter = filter;

    if (SimulateFireBullet(weaponInfo, data, enemy))
    {
        damage = data.currentDamage;
        return true;
    }
    return false;
}

bool RebuiltHandleBulletPenetration(CCSWeaponBaseVData* weaponData, FireBulletData& data)
{
    C_SurfaceData* enter_surface_data = data.enterTrace.GetSurfaceData();
    int enter_material = enter_surface_data->Material;
    float enter_surf_penetration_mod = enter_surface_data->PenetrationModifier;

    data.traceLength += data.enterTrace.Fraction * data.traceLengthRemaining;
    data.currentDamage *= pow(weaponData->m_flRangeModifier(), (data.traceLength * 0.002f));

    if ((data.traceLength > 3000.f) || (enter_surf_penetration_mod < 0.1f))
        data.penetrateCount = 0;

    if (data.penetrateCount <= 0)
        return false;

    Vector dummy;
    C_GameTrace trace_exit;
    C_Ray ray = {};
    //offsets::TraceShape(&ray, data.src, data.enterTrace.EndPos, &data.filter, &trace_exit);
    int tmpPen = 4;
    if(!offsets::TraceToExit(data.enterTrace.EndPos, data.direction, data.src, &data.enterTrace, &trace_exit, 4.f, 90, globals::localPlayerPawn, &tmpPen))
        return false;


    C_SurfaceData* exit_surface_data = trace_exit.GetSurfaceData();
    int exit_material = exit_surface_data->Material;

    float exit_surf_penetration_mod = exit_surface_data->PenetrationModifier;
    float final_damage_modifier = 0.16f;
    float combined_penetration_modifier = 0.0f;

    if (((data.enterTrace.Contents & 0xFFFFFFFD) != 0) || (enter_material == 89) || (enter_material == 71))
    {
        combined_penetration_modifier = 3.0f;
        final_damage_modifier = 0.05f;
    }
    else
    {
        combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
    }

    if (enter_material == exit_material)
    {
        if (exit_material == 87 || exit_material == 85)
            combined_penetration_modifier = 3.0f;
        else if (exit_material == 76)
            combined_penetration_modifier = 2.0f;
    }

    float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
    float v35 = (data.currentDamage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weaponData->m_flPenetration()) * 1.25f);
    float thickness = (trace_exit.EndPos - data.enterTrace.EndPos).Length();

    thickness *= thickness;
    thickness *= v34;
    thickness /= 24.0f;

    float lost_damage = fmaxf(0.0f, v35 + thickness);

    if (lost_damage > data.currentDamage)
        return false;

    if (lost_damage >= 0.0f)
        data.currentDamage -= lost_damage;

    if (data.currentDamage < 1.0f)
        return false;

    data.src = trace_exit.EndPos;
    data.penetrateCount--;

    return true;
}
bool AutoWall::SimulateFireBullet(CCSWeaponBaseVData* weaponData, FireBulletData& data, C_CSPlayerPawnBase* enemy)
{
    C_CSPlayerPawnBase* entityToSkip = nullptr;
    data.penetrateCount = 4;
    data.traceLength = 0.0f;
    data.currentDamage = (float)weaponData->m_nDamage();

    while (data.penetrateCount > 0 && data.currentDamage >= 1.f)
    {
        data.traceLengthRemaining = weaponData->m_flRange() - data.traceLength;

        trace::C_Ray ray;
        //Why is this needed?
        ray.UnkType = 0;
        ray.Start = ray.End = ray.Mins = ray.Maxs = Vector();
        data.filter.V4 |= 2;

        Vector end = data.src + data.direction * data.traceLengthRemaining;

        offsets::TraceShape(&ray, data.src, end, &data.filter, &data.enterTrace);

        Vector newEndPoint = end + data.direction * 40.f;

        offsets::ClipTraceToPlayers(data.src, newEndPoint, &data.filter, &data.enterTrace, 0.f, 60.f, (1.F / (data.src - end).Length()) * (data.enterTrace.EndPos - data.src).Length());

        C_CSPlayerPawnBase* hitEntity = data.enterTrace.HitEntity;

        if (!hitEntity) break;

        if (hitEntity->IsPlayerPawn())
        {
            entityToSkip = hitEntity;
        }

        if (data.enterTrace.Fraction == 1.f) break;

        trace::C_SurfaceData* Surface = data.enterTrace.GetSurfaceData();
        int material = Surface->Material;
        bool bHitGrate = (data.enterTrace.Contents & 0x2000) != 0;

        int hitgroup = data.enterTrace.GetHitGroup();

        if (hitgroup >= HITGROUP_HEAD && hitgroup <= HITGROUP_NECK)
        {
            data.traceLength += data.enterTrace.Fraction * data.traceLengthRemaining;

            data.currentDamage *= pow(weaponData->m_flRangeModifier(), data.traceLength * 0.002f);
            ScaleDamage(enemy, weaponData, data.currentDamage, hitgroup);
            return true;
        }

        int zero = 0;

        if (!RebuiltHandleBulletPenetration(weaponData, data))
            break;

    }
    return false;
}


