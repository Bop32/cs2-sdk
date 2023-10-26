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
            trace::C_Ray ray {};
            trace::C_TraceFilter filter(0x1C3003, 0, 0, 0);

            offsets::TraceShape(&ray, end, traceEnd, &filter, &exitTrace);
            //CDebugOverlayGameSystem::Get()->AddTextOverlay(exitTrace.EndPos, 0.01f, 0.5f, IM_COL32(255, 255, 255, 255), "End");

            bool v27 = *(( BYTE* )&exitTrace + 0xB7);


            if (exitTrace.bStartSolid && exitTrace.nSurfaceFlags & 0x0080)
            {
                trace::C_TraceFilter filter(0x1C3003, 0, exitTrace.pHitEntity, 0);
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

bool RebuiltHandleBulletPenetration(CCSWeaponBaseVData* weaponData, FireBulletData& data)
{
    C_SurfaceData* enterSurfaceData = data.enterTrace.GetSurfaceData();
    int enterMateiral = enterSurfaceData->Material;
    float enterSurfPenetrationMod = enterSurfaceData->PenetrationModifier;

    data.traceLength += data.enterTrace.flFraction * data.traceLengthRemaining;
    data.currentDamage *= pow(weaponData->m_flRangeModifier(), (data.traceLength * 0.002f));

    if ((data.traceLength > 3000.f) || (enterSurfPenetrationMod < 0.1f))
        data.penetrateCount = 0;

    if (data.penetrateCount <= 0)
        return false;

    C_GameTrace traceExit;

    Vector dummy;

    bool test;
    if (!offsets::TraceToExit(data.enterTrace.vecEnd, data.direction, dummy, &data.enterTrace, &traceExit, 4, 90.0f, nullptr, &test))
        return false;
    //if (!RebuiltTraceToExit(dummy, data.enterTrace, data.enterTrace.EndPos, data.direction, traceExit))
        //return false;

    C_SurfaceData* exitSurfaceData = traceExit.GetSurfaceData();
    int exitMaterial = exitSurfaceData->Material;

    float exitSurfPenetrationMod = exitSurfaceData->PenetrationModifier;
    float finalDamageModifer = 0.16f;
    float combinedPenetrationModifer = 3.0f;

    if (enterMateiral == exitMaterial)
    {
        if (((exitMaterial - 85) & 0xFFFFFFFD) != 0)
        {
            if (exitMaterial == 76)
            {
                combinedPenetrationModifer = 2.0f;
            }
        }
        else
        {
            combinedPenetrationModifer = 3.0f;
        }
    }


    float penetrationModifer = fmaxf(1.0 / combinedPenetrationModifer, 0.0);
    float v35 = ((fmaxf(3.75 / weaponData->m_flPenetration(), 0.0) * penetrationModifer) * 3.0) + (data.currentDamage * finalDamageModifer);
    //float penetrationWeaponModifer = (data.currentDamage * finalDamageModifer) + penetrationModifer * 3.0f * fmaxf(0.0f, (3.0f / weaponData->m_flPenetration()) * 1.25f);

    //Calculating thickness is correct as if you sqrt it and multiply by 2.54 (inches to cm) you get what valve gets, damage values are wrong.
    float thickness = (traceExit.vecEnd.z - data.enterTrace.vecEnd.z) * (traceExit.vecEnd.z - data.enterTrace.vecEnd.z)
        + (traceExit.vecEnd.y - data.enterTrace.vecEnd.y) * (traceExit.vecEnd.y - data.enterTrace.vecEnd.y)
        + (traceExit.vecEnd.x - data.enterTrace.vecEnd.x) * (traceExit.vecEnd.x - data.enterTrace.vecEnd.x);

    CDebugOverlayGameSystem::Get()->AddTextOverlay(traceExit.vecEnd, 0.01f, 1, IM_COL32(255, 255, 255, 255), "End");
    CDebugOverlayGameSystem::Get()->AddTextOverlay(data.enterTrace.vecEnd, 0.01f, 1, IM_COL32(255, 255, 255, 255), "Start");

    float distance = sqrtf(thickness);

    thickness *= penetrationModifer;
    thickness *= 0.041666668;
    thickness += v35;

    float lostDamage = thickness;
    CDebugOverlayGameSystem::Get()->AddTextOverlay(Vector(traceExit.vecEnd.x, traceExit.vecEnd.y, traceExit.vecEnd.z + 15), 0.001f, 1, IM_COL32(255, 255, 255, 255), std::to_string(distance * 2.54f).c_str());

    if (lostDamage > data.currentDamage)
        return false;

    if (lostDamage >= 0.0f)
        data.currentDamage -= lostDamage;

    if (data.currentDamage < 1.0f)
        return false;


    data.src = traceExit.vecEnd;
    data.penetrateCount--;

    return true;
}
bool AutoWall::SimulateFireBullet(CCSWeaponBaseVData* weaponData, FireBulletData& data, C_CSPlayerPawnBase* enemy)
{
    C_CSPlayerPawnBase* entityToSkip = nullptr;
    data.penetrateCount = 4;
    data.traceLength = 0.0f;
    data.currentDamage = weaponData->m_nDamage();

    while (data.penetrateCount > 0 && data.currentDamage >= 1.f)
    {
        data.traceLengthRemaining = weaponData->m_flRange() - data.traceLength;

        trace::C_Ray ray;
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


