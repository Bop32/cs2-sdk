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

using namespace trace;

bool aimbot::HitChance(C_CSPlayerPawnBase* localPlayer)
{
    constexpr float HITCHANCE_MAX = 100.f;
    constexpr int   SEED_MAX = 255;
    Vector     start { localPlayer->GetEyePosition() }, end, fwd, right, up, dir, wep_spread;
    float      inaccuracy, spread;
    trace::C_GameTrace tr;
    return true;
}

static std::vector<uint32_t> bones;

void aimbot::RunAimbot(CUserCmd* cmd, C_CSPlayerPawnBase* localPlayer)
{
    if (!CEngineClient::Get()->IsInGame()) return;

    auto entitySystem = CGameEntitySystem::Get();

    Vector localPlayerViewAngles = cmd->base->view->angles;

    Vector target;

    Vector localPlayerEyePosition = localPlayer->GetEyePosition();
    Vector angle;
    float aimbotFov = g_Vars.m_AimbotFov * 2.f;

    float bestDamage = 0;
    float currentDamage = 0;

    bones.emplace_back(4);
    bones.emplace_back(6);

    C_AttributeContainer* pAttributeContainer = localPlayer->m_pWeaponServices()->m_hActiveWeapon().Get()->m_AttributeManager();

    if (!pAttributeContainer) return;

    weapon::C_EconItemView* pItemView = pAttributeContainer->m_Item();

    if (!pItemView) return;

    weapon::CCSWeaponBaseVData* weaponInfo = pItemView->GetWeaponInfo();

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

        for (uint32_t bone : bones)
        {
            enemyPawn->GetBonePosition(bone, bone_position, bone_rotation);

            C_TraceFilter filter(0x1C1003, localPlayer, nullptr, 4);
            C_Ray ray = {};
            C_GameTrace trace = {};

            offsets::TraceShape(&ray, localPlayerEyePosition, bone_position, &filter, &trace);

            if (trace.HitEntity != enemyPawn && trace.Fraction < 0.97f) continue;

            if (!AutoWall::CanHit(enemyPawn, weaponInfo, trace.GetHitGroup(), trace.Fraction, currentDamage)) continue;

            angle = CMath::Get().CalculateAngle(localPlayerEyePosition, bone_position, localPlayerViewAngles);

            angle.Clamp();

            auto fov = hypotf(angle.x, angle.y);

            if (fov < aimbotFov && currentDamage > bestDamage)
            {
                target = bone_position;
                aimbotFov = fov;
                bestDamage = currentDamage;
            }
        }
    }

    bones.clear();
    if (target.IsZero()) return;

    auto& aimPunch = localPlayer->m_aimPunchCache();
    localPlayerViewAngles += angle - aimPunch.m_Data[aimPunch.m_Size - 1] * 2;

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

        cmd->buttons |= CUserCmd::IN_ATTACK;
    }
}

