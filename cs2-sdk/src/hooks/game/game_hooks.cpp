#include "pch.hpp"

#include <hooks/game/game_hooks.hpp>
#include <logger/logger.hpp>

#include <signatures/signatures.hpp>
#include <constants/constants.hpp>

#include <hacks/esp/esp.hpp>
#include <cache/cache.hpp>
#include <menu/menu.hpp>
#include <math/math.hpp>
#include <hook/hook.hpp>

#include <interfaces/gameentitysystem.hpp>
#include <interfaces/source2client.hpp>

#include <input/ccsgoinput.hpp>
#include <bindings/playerpawn.hpp>
#include <hacks/misc/misc.hpp>
#include <globals/globals.hpp>
#include <bindings/trace.hpp>
#include <offets/offsets.hpp>

#include <hacks/aimbot/aimbot.hpp>
#include <vars/vars.hpp>
#include <material/material.hpp>
#include <interfaces/CMaterialSystem2.hpp>
#include <material/keystring.hpp>
#include <bindings/c_overrideview.hpp>
#include <interfaces/CLocalize.hpp>
#include <interfaces/CResourceSystem.hpp>
#include <hacks/anti_aim/anti_aim.hpp>

using namespace globals;
static CHook g_MouseInputEnabled;
static bool hkMouseInputEnabled(void* rcx) { return CMenu::Get().IsOpen() ? false : g_MouseInputEnabled.CallOriginal<bool>(rcx); }

static CHook g_OnAddEntity;
static void* hkOnAddEntity(void* rcx, CEntityInstance* inst, CBaseHandle handle)
{
    CMatchCache::Get().AddEntity(inst, handle);
    return g_OnAddEntity.CallOriginal<void*>(rcx, inst, handle);
}

static CHook g_OnRemoveEntity;
static void* hkOnRemoveEntity(void* rcx, CEntityInstance* inst, CBaseHandle handle)
{
    CMatchCache::Get().RemoveEntity(inst, handle);
    return g_OnRemoveEntity.CallOriginal<void*>(rcx, inst, handle);
}

static CHook g_LevelInit;
static __int64 hkLevelInit(void* rcx)
{
    GlobalVars = *signatures::GlobalVars.GetPtrAs<globals::CGlobalVarsBase**>();
    localPlayerController = CGameEntitySystem::GetLocalPlayerController();

    return g_LevelInit.CallOriginal<__int64>(rcx);
}


static CHook g_DrawObject;

static void* hkDrawObject(void* animtable_scene_object, void* dx11, void* data,
    int unknown_bool, void* scene_view, void* scene_layer, void* unknown_pointer, void* unknown)
{
    using namespace material;
    const int CT_MODEL = 113;
    const int T_MODEL = 104;
    const int ARM = 38;
    CMaterial2* material = *( CMaterial2** )(( uintptr_t )data + 0x18);

    static CMaterial2* invisible_material = CreateMaterialInvisible();
    CMaterial2* visible_material = CreateMaterialVisible();

    void* objectInfo = *( void** )(( uintptr_t )data + 0x48);
    int id = *( int* )(( uintptr_t )objectInfo + 0xb0);

    auto name = material->GetName();

    if (!strstr(name, "characters/model") || !(id == CT_MODEL || id == T_MODEL || id == ARM))
        return g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer, unknown_pointer, unknown);

    if (localPlayerController)
    {
        if (localPlayerController->m_bPawnIsAlive() && localPlayerPawn)
        {
            if (localPlayerPawn->m_iTeamNum() == 2 && id == T_MODEL)
                return g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer,
                unknown_pointer, unknown);
            if (localPlayerPawn->m_iTeamNum() == 3 && id == CT_MODEL)
                return g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer,
                unknown_pointer, unknown);
        }
    }


    switch (id)
    {
        case CT_MODEL:
        {
            if (g_Vars.m_InvisibleChams)
            {
                *( CMaterial2** )(( uintptr_t )data + 0x18) = invisible_material;
                SetColor(data, g_Vars.m_PlayerInvisChamsColor);
                g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer, unknown_pointer, unknown);
            }

            if (g_Vars.m_VisibleChams)
            {
                *( CMaterial2** )(( uintptr_t )data + 0x18) = visible_material;
                SetColor(data, g_Vars.m_PlayerVisibleChamsColor);
                g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer, unknown_pointer, unknown);
            }
            break;
        }
        case T_MODEL:
        {
            if (g_Vars.m_InvisibleChams)
            {
                *( CMaterial2** )(( uintptr_t )data + 0x18) = invisible_material;
                SetColor(data, g_Vars.m_PlayerInvisChamsColor);
                g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer, unknown_pointer, unknown);
            }

            if (g_Vars.m_VisibleChams)
            {
                *( CMaterial2** )(( uintptr_t )data + 0x18) = visible_material;
                SetColor(data, g_Vars.m_PlayerVisibleChamsColor);
                g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer, unknown_pointer, unknown);
            }
            break;
        }
        default:
            break;
    }
    return g_DrawObject.CallOriginal<void*>(animtable_scene_object, dx11, data, unknown_bool, scene_view, scene_layer, unknown_pointer, unknown);
}

static CHook g_BulletMessage;

static void* hkModifyBulletMessage(CSGOInputMessage* inputMessage, CSGOInputHistoryEntryPB* inputHistoryEntry, bool verify, int64_t a3, int64_t a4, C_CSPlayerPawnBase* player)
{
    if (g_Vars.m_RapidFire)
    {
        inputMessage->m_player_tick_count = 1;
        inputMessage->m_target_head_position = aimbotData.shotPosition;
        inputMessage->m_shoot_position = aimbotData.shotPosition;
        inputMessage->m_target_angle = aimbotData.angle;
        inputMessage->m_target_abs_origin = aimbotData.shotPosition;
        inputMessage->m_view_angles = player->GetEyePosition();
        inputHistoryEntry->m_target_index = inputMessage->m_target_index;
    }

    return g_BulletMessage.CallOriginal<void*>(inputMessage, inputHistoryEntry, verify, a3, a4, player);
}
static CHook g_CreateMove;
static bool hkCreateMove(CCSGOInput* this_ptr, int a1, int a2)
{
    g_CreateMove.CallOriginal<bool>(this_ptr, a1, a2);

    cmd = this_ptr->GetUserCmd();

    if (!cmd) return false;

    if (!localPlayerController)
    {
        localPlayerController = CGameEntitySystem::GetLocalPlayerController();
    }

    if (!localPlayerController || !localPlayerController->m_bPawnIsAlive()) return false;

    localPlayerPawn = localPlayerController->m_hPawn().Get();

    if (!localPlayerPawn) return false;

    if (!globals::GlobalVars)
    {
        globals::GlobalVars = *signatures::GlobalVars.GetPtrAs<globals::CGlobalVarsBase**>();
    }

    Vector old_angles = cmd->base->view->angles;
    float old_fmove = cmd->base->m_forwardmove;
    float old_smove = cmd->base->m_rightmove;


    if (g_Vars.m_Aimbot)
    {

        aimbot::RunAimbot(cmd);
    }

    if (g_Vars.m_Bhop)
    {
        misc::BunnyHop(cmd, localPlayerPawn);
    }


    AntiAim::RunAntiAim(cmd, this_ptr);
    CMath::Get().CorrectMovement(old_angles, cmd, old_fmove, old_smove);

    return false;
}


static float originalFOV = 0;
static CHook g_OverrideView;
static void hkOverrideView(void* input, CViewSetup* view)
{
    return g_OverrideView.CallOriginal<void>(input, view);
}

static CHook g_IsDemoOrHLTV;
static char hkIsDemoOrHLTV()
{
    return 1;
}

static CHook g_GetMatricesForView;
static void hkGetMatricesForView(void* rcx, void* view, VMatrix* pWorldToView, VMatrix* pViewToProjection, VMatrix* pWorldToProjection,
    VMatrix* pWorldToPixels)
{
    g_GetMatricesForView.CallOriginal<void>(rcx, view, pWorldToView, pViewToProjection, pWorldToProjection, pWorldToPixels);

    CMath::Get().UpdateViewMatrix(pWorldToProjection);
    CESP::Get().Update();
}


static CHook g_FrameStageNotify;

static void hkFrameStageNotify(void* rcx, int frameStage)
{
    return g_FrameStageNotify.CallOriginal<void>(rcx, frameStage);
}

void CGameHooks::Initialize()
{
    SDK_LOG_PROLOGUE();

    CMatchCache::Get().Initialize();

    g_MouseInputEnabled.VHook(CCSGOInput::Get(), 11 + LINUX_OFFSET(1), SDK_HOOK(hkMouseInputEnabled));
    g_OnAddEntity.VHook(CGameEntitySystem::Get(), 14 + LINUX_OFFSET(1), SDK_HOOK(hkOnAddEntity));
    g_OnRemoveEntity.VHook(CGameEntitySystem::Get(), 15 + LINUX_OFFSET(1), SDK_HOOK(hkOnRemoveEntity));
    g_GetMatricesForView.Hook(signatures::GetMatricesForView.GetPtrAs<void*>(), SDK_HOOK(hkGetMatricesForView));
    g_CreateMove.VHook(CCSGOInput::Get(), 5, SDK_HOOK(hkCreateMove));
    g_IsDemoOrHLTV.Hook(signatures::GetIsDemoOrHLTV.GetPtrAs<void*>(), SDK_HOOK(hkIsDemoOrHLTV));
    g_LevelInit.Hook(signatures::LevelInit.GetPtrAs<void*>(), SDK_HOOK(hkLevelInit));
    g_DrawObject.Hook(signatures::DrawObjectHook.GetPtrAs<void*>(), SDK_HOOK(hkDrawObject));
    g_OverrideView.Hook(signatures::OverrideView.GetPtrAs<void*>(), SDK_HOOK(hkOverrideView));
    g_FrameStageNotify.Hook(signatures::FrameStageNotify.GetPtrAs<void*>(), SDK_HOOK(hkFrameStageNotify));
    g_BulletMessage.Hook(signatures::BulletMessage.GetPtrAs<void*>(), SDK_HOOK(hkModifyBulletMessage));

    //Sets Cl_Bob_Lower_Amt to 0.
    float* value = signatures::CL_Bob_Lower_Amt.GetPtrAs<float*>();

    DWORD oldProtect;
    VirtualProtect(value, sizeof(*value), PAGE_READWRITE, &oldProtect);
    *value = 0;
}
