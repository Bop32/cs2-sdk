#include <pch.hpp>
#include <hacks/aimbot/aimbot.hpp>
#include <math/math.hpp>
#include <interfaces/engineclient.hpp>
#include <interfaces/gameentitysystem.hpp>
#include <bindings/playerpawn.hpp>
#include <bindings/playercontroller.hpp>
#include <input/cusercmd.hpp>
#include <vector>
#include <input/ccsgoinput.hpp>
#include <logger/logger.hpp>
#include <offets/offsets.hpp>
#include <vars/vars.hpp>
#include <bindings/trace.hpp>
#include <globals/globals.hpp>
#include <hacks/aimbot/autowall.hpp>
#include <debugoverlay/CDebugOverlayGameSystem.hpp>

using namespace trace;
using namespace globals;
bool aimbot::HitChance(C_CSPlayerPawnBase* enemy, Vector angle, C_BasePlayerWeapon* weapon, CCSWeaponBaseVData* weaponInfo, float weaponRange)
{
    constexpr float HITCHANCE_MAX = 100.f;
    constexpr int   SEED_MAX = 255;
    Vector     start { localPlayerPawn->GetEyePosition() }, end, fwd, right, up, dir, wep_spread;
    float      inaccuracy, spread;
    C_GameTrace tr;
    size_t     total_hits { }, needed_hits { ( size_t )std::ceil((g_Vars.m_HitChanceValue * SEED_MAX) / HITCHANCE_MAX) };
    C_TraceFilter filter(0x1C300B, localPlayerPawn, 0, 3);
    C_Ray ray;

    CMath::Get().AngleVectors(angle, &fwd, &right, &up);

    inaccuracy = weapon->GetInaccuracy();
    spread = weapon->GetSpread();

    for (int i { }; i <= SEED_MAX; ++i)
    {
        wep_spread = weapon->CalculateSpread(weaponInfo, i, inaccuracy, spread);

        dir = (fwd + (right * wep_spread.x) + (up * wep_spread.y)).Normalize();

        end = start + (dir * weaponRange);

        offsets::TraceShape(&ray, start, end, &filter, &tr);

        if (tr.pHitEntity == enemy)
            ++total_hits;

        if (total_hits >= needed_hits)
            return true;

        if ((SEED_MAX - static_cast<unsigned long long>(i) + total_hits) < needed_hits)
            return false;
    }

    return false;
}

static std::vector<uint32_t> bones;

bool WeaponCanFire(C_BasePlayerWeapon* weapon, CUserCmd* cmd)
{
    float flServerTime = ( float )(localPlayerController->m_nTickBase() * globals::GlobalVars->interval_per_tick);

    auto weaponAttackTime = weapon->m_nNextPrimaryAttackTick() * globals::GlobalVars->interval_per_tick;
    return weaponAttackTime > flServerTime;
}
void aimbot::RunAimbot(CUserCmd* cmd)
{
    if (!CEngineClient::Get()->IsInGame()) return;

    auto entitySystem = CGameEntitySystem::Get();

    Vector localPlayerViewAngles = cmd->base->view->angles;

    aimbotData.enemy = nullptr;
    aimbotData.shotPosition = {};
    aimbotData.angle = {};
    aimbotData.canFire = false;
    Vector localPlayerEyePosition = localPlayerPawn->GetEyePosition();
    float aimbotFov = g_Vars.m_AimbotFov * 2.f;

    float bestDamage = 0;
    float currentDamage = 0;

    //Head
    if (g_Vars.m_HitBoxes.at("Head"))
    {
        bones.emplace_back(6);
    }

    //Chest
    if (g_Vars.m_HitBoxes.at("Chest"))
    {
        bones.emplace_back(3);
        bones.emplace_back(4);

    }
    //Pelivs
    if (g_Vars.m_HitBoxes.at("Stomach"))
    {
        bones.emplace_back(0);
        bones.emplace_back(1);
        bones.emplace_back(2);
        bones.emplace_back(22);
        bones.emplace_back(25);

    }
    //Shoulders, Arms and hands?
    if (g_Vars.m_HitBoxes.at("Arms"))
    {
        bones.emplace_back(8);
        bones.emplace_back(9);
        bones.emplace_back(10);
        bones.emplace_back(13);
        bones.emplace_back(14);
        bones.emplace_back(15);

    }
    //Legs and toes
    if (g_Vars.m_HitBoxes.at("Legs"))
    {
        bones.emplace_back(23);
        bones.emplace_back(24);
        bones.emplace_back(26);
        bones.emplace_back(27);
    }

    C_BasePlayerWeapon* currentWeapon = localPlayerPawn->m_pWeaponServices()->m_hActiveWeapon().Get();

    if (!currentWeapon) return;

    C_AttributeContainer* pAttributeContainer = currentWeapon->m_AttributeManager();

    if (!pAttributeContainer) return;

    C_EconItemView* pItemView = pAttributeContainer->m_Item();

    if (!pItemView) return;

    CCSWeaponBaseVData* weaponInfo = pItemView->GetWeaponInfo();

    if (!weaponInfo) return;

    for (int i = 0; i < CGameEntitySystem::GetHighestEntityIndex(); i++)
    {
        CCSPlayerController* enemyController = reinterpret_cast< CCSPlayerController* >(CGameEntitySystem::GetBaseEntity(i));

        if (!enemyController || !enemyController->IsPlayerController() ||
            enemyController->m_iTeamNum() == localPlayerPawn->m_iTeamNum() ||
            !enemyController->m_bPawnIsAlive()) continue;

        C_CSPlayerPawnBase* enemyPawn = enemyController->m_hPawn().Get();

        if (!enemyPawn) continue;

        Vector bone_position {};
        Vector bone_rotation {};

        for (uint32_t bone : bones)
        {
            enemyPawn->GetBonePosition(bone, bone_position, bone_rotation);

            Vector out;

            CMath::Get().VectorAngles(bone_position - localPlayerEyePosition, out);

            aimbotData.angle = CMath::Get().CalculateAngle(localPlayerEyePosition, bone_position, localPlayerViewAngles);

            aimbotData.angle.Clamp();

            if (!HitChance(enemyPawn, out, currentWeapon, weaponInfo, weaponInfo->m_flRange()))  continue;

            if (!AutoWall::CanHit(enemyPawn, localPlayerEyePosition, bone_position, weaponInfo, currentDamage)) continue;

            auto fov = hypotf(aimbotData.angle.x, aimbotData.angle.y);

            if (fov < aimbotFov && currentDamage > bestDamage)
            {
                aimbotData.enemy = enemyPawn;
                aimbotData.shotPosition = bone_position;
                aimbotFov = fov;
                bestDamage = currentDamage;
            }
        }
    }

    bones.clear();

    if (aimbotData.shotPosition.IsZero()) return;

    aimbotData.angle = CMath::Get().CalculateAngle(localPlayerEyePosition, aimbotData.shotPosition, localPlayerViewAngles);

    aimbotData.angle.Clamp();

    auto& aimPunch = localPlayerPawn->m_aimPunchCache();

    aimbotData.angle -= aimPunch.m_Data[aimPunch.m_Size - 1] * 2;

    if (aimPunch.m_Size == 0) return;

    localPlayerViewAngles += aimbotData.angle;

    cmd->SetSubTickAngles(cmd, localPlayerViewAngles);

    if (!g_Vars.m_SilentAim)
    {
        cmd->base->view->angles = localPlayerViewAngles;
        CCSGOInput::Get()->SetViewAngles(localPlayerViewAngles);
    }


    if (g_Vars.m_AutoFire)
    {

        if (WeaponCanFire(currentWeapon, cmd) || weaponInfo->m_nNumBullets() <= 0)
        {
            aimbotData.canFire = false;
            return;
        }

        aimbotData.canFire = true;
        cmd->buttons |= CUserCmd::IN_ATTACK;
    }
}


