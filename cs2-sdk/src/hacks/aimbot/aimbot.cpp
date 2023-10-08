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

using namespace trace;

bool aimbot::HitChance(C_CSPlayerPawnBase* localPlayer)
{
    constexpr float HITCHANCE_MAX = 100.f;
    constexpr int   SEED_MAX = 255;
    Vector     start { localPlayer->GetEyePosition()}, end, fwd, right, up, dir, wep_spread;
    float      inaccuracy, spread;
    trace::C_GameTrace tr;

}

void aimbot::RunAimbot(CUserCmd* cmd, C_CSPlayerPawnBase* localPlayer)
{
    if (!CEngineClient::Get()->IsInGame()) return;

    auto entitySystem = CGameEntitySystem::Get();

    Vector localPlayerViewAngles = cmd->base->view->angles;

    Vector target;

    Vector localPlayerEyePosition = localPlayer->GetEyePosition();
    Vector angle;
    float aimbotFov = g_Vars.m_AimbotFov * 2.f;

    for (int i = 0; i < CGameEntitySystem::GetHighestEntityIndex(); i++)
    {
        CCSPlayerController* enemyController = reinterpret_cast< CCSPlayerController* >(CGameEntitySystem::GetBaseEntity(i));

        if (!enemyController || !enemyController->IsPlayerController() ||
            enemyController->m_iTeamNum() == localPlayer->m_iTeamNum() ||
            !enemyController->m_bPawnIsAlive()) continue;

        C_CSPlayerPawnBase* enemyPawn = enemyController->m_hPawn().Get();

        if (!enemyPawn) continue;

        Vector bone_position {};
        Vector bone_rotation {};

        enemyPawn->GetBonePosition(6, bone_position, bone_rotation);

        C_TraceFilter filter(0x1C3003, localPlayer, nullptr, 4);
        C_Ray ray = {};
        C_GameTrace trace = {};

        offsets::TraceShape(&ray, localPlayerEyePosition, bone_position, &filter, &trace);

        if (trace.HitEntity != enemyPawn && trace.Fraction < 0.97f) continue;

        angle = CMath::Get().CalculateAngle(localPlayerEyePosition, bone_position, localPlayerViewAngles);

        angle.Clamp();

        auto fov = hypotf(angle.x, angle.y);

        if (fov < aimbotFov)
        {
            target = bone_position;
            aimbotFov = fov;
            break;
        }
    }

    if (target.IsZero()) return;

    localPlayerViewAngles += angle - localPlayer->m_aimPunchAngle() * 2;

    cmd->SetSubTickAngles(cmd, localPlayerViewAngles);

    if (!g_Vars.m_SilentAim)
    {
        cmd->base->view->angles = localPlayerViewAngles;
        CCSGOInput::Get()->SetViewAngles(localPlayerViewAngles);
    }

    if (g_Vars.m_AutoFire)
    {
        auto weaponService = localPlayer->m_pWeaponServices();

        if (!weaponService) return;

        auto activeWeapon = weaponService->m_hActiveWeapon().Get();

        if (!activeWeapon) return;


        if (!globals::GlobalVars)
        {
            globals::GlobalVars = *signatures::GlobalVars.GetPtrAs<globals::CGlobalVarsBase**>();
        }

        if (activeWeapon->m_nNextPrimaryAttackTick() > globals::GlobalVars->tick_count) return;

        cmd->m_buttons |= CUserCmd::IN_ATTACK;
    }
}

