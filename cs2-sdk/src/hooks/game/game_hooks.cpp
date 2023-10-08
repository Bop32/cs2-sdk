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

static CHook g_CreateMove;
static bool hkCreateMove(CCSGOInput* this_ptr, int a1, int a2) {

    g_CreateMove.CallOriginal<bool>(this_ptr, a1, a2);

    CUserCmd* cmd = this_ptr->GetUserCmd();

    if (!cmd) return false;

    auto localPlayerController = CGameEntitySystem::GetLocalPlayerController();

    if (!localPlayerController || !localPlayerController->m_bPawnIsAlive()) return false;

    C_CSPlayerPawnBase* pawn = localPlayerController->m_hPawn().Get();

    if (!pawn) return false;

    //misc::NoRecoil(cmd, pawn);

    if (g_Vars.m_Aimbot)
    {
        aimbot::RunAimbot(cmd, pawn);
        
    }

    misc::BunnyHop(cmd, pawn);

    return false;
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

    globals::local_player = CGameEntitySystem::GetLocalPlayerController();
}
