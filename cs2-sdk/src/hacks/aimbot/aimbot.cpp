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

//static std::vector<uint32_t> bones;


void aimbot::RunAimbot(CUserCmd* cmd, C_CSPlayerPawnBase* localPlayer)
{
    if (!CEngineClient::Get()->IsInGame()) return;

    auto entitySystem = CGameEntitySystem::Get();

    Vector localPlayerViewAngles = cmd->baseCmd->view->angles;

    Vector target;

    Vector bone_position { };
    Vector bone_rotation { };

    Vector localPlayerEyePosition = localPlayer->GetEyePosition();
    Vector angle;
    int aimbotFov = g_Vars.m_AimbotFov * 2;

    for (int i = 0; i < CGameEntitySystem::GetHighestEntityIndex(); i++)
    {
        CCSPlayerController* enemyController = reinterpret_cast< CCSPlayerController* >(CGameEntitySystem::GetBaseEntity(i));

        if (!enemyController || !enemyController->IsPlayerController() || enemyController->m_iTeamNum() == localPlayer->m_iTeamNum() || !enemyController->m_bPawnIsAlive()) continue;

        C_CSPlayerPawnBase* enemyPawn = enemyController->m_hPawn().Get();

        if (!enemyPawn) continue;

        enemyPawn->GetBonePosition(6, bone_position, bone_rotation);

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

    if (target.IsZero())
    {
        return;
    }
    using namespace trace;

    C_TraceFilter filter(0x1C3003, localPlayer, nullptr, 4);
    C_Ray ray = {};
    C_GameTrace trace = {};

    Vector eyePosition = localPlayer->GetEyePosition();

    offsets::TraceShape(&ray, eyePosition, target, &filter, &trace);

    localPlayerViewAngles += angle;
    cmd->SetSubTickAngles(cmd, localPlayerViewAngles);

    //cmd->m_buttons |= cmd->IN_ATTACK;
    if (!g_Vars.m_SilentAim)
    {
        cmd->baseCmd->view->angles = localPlayerViewAngles;
        CCSGOInput::Get()->SetViewAngles(localPlayerViewAngles);
    }
}
