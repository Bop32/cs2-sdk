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
using namespace globals;
bool aimbot::HitChance(C_CSPlayerPawnBase* localPlayerController)
{
    constexpr float HITCHANCE_MAX = 100.f;
    constexpr int   SEED_MAX = 255;
    Vector     start { localPlayerController->GetEyePosition() }, end, fwd, right, up, dir, wep_spread;
    float      inaccuracy, spread;
    C_GameTrace tr;
    return true;
}

static std::vector<uint32_t> bones;

void aimbot::RunAimbot(CUserCmd* cmd)
{
    if (!CEngineClient::Get()->IsInGame()) return;

    auto entitySystem = CGameEntitySystem::Get();

    Vector localPlayerViewAngles = cmd->base->view->angles;

    Vector target;

    Vector localPlayerEyePosition = localPlayerPawn->GetEyePosition();
    Vector angle;
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

    auto currentWeapon = localPlayerPawn->m_pWeaponServices()->m_hActiveWeapon().Get();

    C_AttributeContainer* pAttributeContainer = currentWeapon->m_AttributeManager();

    if (!pAttributeContainer) return;

    C_EconItemView* pItemView = pAttributeContainer->m_Item();

    if (!pItemView) return;

    CCSWeaponBaseVData* weaponInfo = pItemView->GetWeaponInfo();

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

            /*
            we can maybe have it so the user chooses to have autowall on or off so we use this shit.
            C_TraceFilter filter(0x1C1003, localPlayerController, nullptr, 4);
            C_Ray ray = {};
            C_GameTrace trace = {};

            offsets::TraceShape(&ray, localPlayerEyePosition, bone_position, &filter, &trace);

            if (trace.HitEntity != enemyPawn && trace.Fraction < 0.97f) continue;
            */

            if (!AutoWall::CanHit(enemyPawn, localPlayerEyePosition, bone_position, weaponInfo, currentDamage)) continue;

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

    angle = CMath::Get().CalculateAngle(localPlayerEyePosition, target, localPlayerViewAngles);

    angle.Clamp();

    auto& aimPunch = localPlayerPawn->m_aimPunchCache();

    if(aimPunch.m_Size == 0) return;

    localPlayerViewAngles += angle - aimPunch.m_Data[aimPunch.m_Size - 1] * 2;

    cmd->SetSubTickAngles(cmd, localPlayerViewAngles);

    if (!g_Vars.m_SilentAim)
    {
        cmd->base->view->angles = localPlayerViewAngles;
        CCSGOInput::Get()->SetViewAngles(localPlayerViewAngles);
    }

    if (g_Vars.m_AutoFire)
    {
        if (!globals::GlobalVars)
        {
            globals::GlobalVars = *signatures::GlobalVars.GetPtrAs<globals::CGlobalVarsBase**>();
        }
        
        //This is needed as for community servers you need to compensate tickBase prediction errors. (I think)
        auto tickDifference = std::abs(currentWeapon->m_nNextPrimaryAttackTick() - (int)localPlayerController->m_nTickBase());

        if (currentWeapon->m_nNextPrimaryAttackTick() + tickDifference > globals::GlobalVars->tick_count) return;

        cmd->buttons |= CUserCmd::IN_ATTACK;
    }
}

