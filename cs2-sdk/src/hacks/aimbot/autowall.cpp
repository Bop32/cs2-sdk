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
    trace::C_TraceFilter filter(0x1C3003, globals::localPlayerPawn, nullptr, 3);
    data.filter = filter;
    if (SimulateFireBullet(weaponInfo, data, enemy))
    {
        damage = data.currentDamage;
        return true;
    }
    return false;
}

bool RebuiltTraceToExit(Vector start, Vector direction, Vector& wallEnd, trace::C_GameTrace enterTrace, trace::C_GameTrace& exitTrace, float step, float maxRange, void* localPlayer, int* didHit)
{

    float currentDistance {};
    int firstContents {};

    while (currentDistance <= maxRange)
    {
        currentDistance += step;

        wallEnd = start + (direction * currentDistance);

        Vector traceEnd = wallEnd - (direction * step);

        int contents = offsets::GetContents(wallEnd, 0x1C3003, 7);

        if (!firstContents)
            firstContents = contents;

        if ((contents & 0x1C3001) == 0 || (contents & 2) != 0 && firstContents != contents)
        {
            trace::C_Ray ray {};
            trace::C_TraceFilter filter(0x1C3001, globals::localPlayerPawn, 0, 0);

            CDebugOverlayGameSystem::Get()->AddLineOverlay(wallEnd, traceEnd, IM_COL32(255,255,255,255), false, 0.01f);
            offsets::TraceShape(&ray, wallEnd, traceEnd, &filter, &exitTrace);
            //offsets::TraceRay(&ray, &enterTrace, &filter, 0, &exitTrace);

            if (exitTrace.allSolid && exitTrace.GetHitGroup())
            {
                return 0;
            }
            else if (exitTrace.Fraction < 1.f)
            {
                if (((exitTrace.surface_flags >> 7) & 1) && !((exitTrace.surface_flags >> 7) & 1))
                    continue;

                Vector normal = exitTrace.normal;

                if (normal.x * direction.x + normal.y * direction.y + normal.z * direction.z <= 1.f)
                {
                    wallEnd = wallEnd - (direction * step * exitTrace.Fraction);
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

    data.traceLength += data.enterTrace.Fraction * data.traceLengthRemaining;
    data.currentDamage *= pow(weaponData->m_flRangeModifier(), (data.traceLength * 0.002f));

    if ((data.traceLength > 3000.f) || (enterSurfPenetrationMod < 0.1f))
        data.penetrateCount = 0;

    if (data.penetrateCount <= 0)
        return false;

    C_GameTrace traceExit;
    int didHit;

    if (!RebuiltTraceToExit(data.src, data.direction, data.enterTrace.EndPos, data.enterTrace, traceExit, 4.0f, 90.f, nullptr, &didHit))
        return false;


    C_SurfaceData* exitSurfaceData = traceExit.GetSurfaceData();
    int exitMaterial = exitSurfaceData->Material;

    float exitSurfPenetrationMod = exitSurfaceData->PenetrationModifier;
    float finalDamageModifer = 0.16f;
    float combinedPenetrationModifer = 0.0f;

    if (((data.enterTrace.Contents & 0x1C3001) != 0) || (enterMateiral == 89) || (enterMateiral == 71))
    {
        combinedPenetrationModifer = 3.0f;
        finalDamageModifer = 0.05f;
    }
    else
    {
        combinedPenetrationModifer = (enterSurfPenetrationMod + exitSurfPenetrationMod) * 0.5f;
    }

    if (enterMateiral == exitMaterial)
    {
        if (((exitMaterial - 85) & 0xFFFFFFFD) != 0)
        {
            if (exitMaterial == 76)
            {
                combinedPenetrationModifer = 3.0f;
            }
        }
        else
        {
            combinedPenetrationModifer = 2.0f;
        }
    }

    float v34 = fmaxf(0.f, 1.0f / combinedPenetrationModifer);
    float v35 = (data.currentDamage * finalDamageModifer) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weaponData->m_flPenetration()) * 1.25f);
    float thickness = (traceExit.EndPos - data.enterTrace.EndPos).Length();

    thickness *= thickness;
    thickness *= v34;
    thickness /= 24.0f;

    float lostDamage = fmaxf(0.0f, v35 + thickness);

    if (lostDamage > data.currentDamage)
        return false;

    if (lostDamage >= 0.0f)
        data.currentDamage -= lostDamage;

    if (data.currentDamage < 1.0f)
        return false;

    data.src = traceExit.EndPos;
    data.penetrateCount--;

    return true;
}
bool AutoWall::SimulateFireBullet(CCSWeaponBaseVData* weaponData, FireBulletData& data, C_CSPlayerPawnBase* enemy)
{
    C_CSPlayerPawnBase* entityToSkip = nullptr;
    data.penetrateCount = 4;
    data.traceLength = 0.0f;
    data.currentDamage = ( float )weaponData->m_nDamage();

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

        if (!RebuiltHandleBulletPenetration(weaponData, data))
            break;

    }
    return false;
}


