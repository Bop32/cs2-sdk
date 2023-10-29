#pragma once

#include <sigscan/sigscan.hpp>

namespace signatures
{
    extern CSigScan GetBaseEntity, GetHighestEntityIndex;
    extern CSigScan GetLocalPlayer;
    extern CSigScan GetEntityHandle;
    extern CSigScan GetCSGOInput;
    extern CSigScan GetIsDemoOrHLTV;
    extern CSigScan GetMatricesForView;
    extern CSigScan GetHitboxSet;
    extern CSigScan HitboxToWorldTransforms;
    extern CSigScan GetBonePosition;
    extern CSigScan GetBoneFlags;
    extern CSigScan GetBoneParent;
    extern CSigScan SetViewAngles;
    extern CSigScan GetViewAngles;
    extern CSigScan GetEntityHandle;
    extern CSigScan GetSurfaceData;
    extern CSigScan TraceShape;
    extern CSigScan TraceManager;
    extern CSigScan GetControllerBasedOnIndex;
    extern CSigScan GlobalVars;
    extern CSigScan LevelInit;
    extern CSigScan BoneParent;
    extern CSigScan DrawObjectHook;
    extern CSigScan FindKeyHook;
    extern CSigScan SetMaterialFunctionHook;
    extern CSigScan SetMaterialShaderHook;
    extern CSigScan OverrideView;
    extern CSigScan CL_Bob_Lower_Amt;
    extern CSigScan WeaponInfo;
    extern CSigScan ClipTraceToPlayers;
    extern CSigScan HandleBulletPenetration;
    extern CSigScan FrameStageNotify;
    extern CSigScan TraceToExit;
    extern CSigScan GetDamageToPoint;
    extern CSigScan GetContents;
    extern CSigScan InitializeTrace;
    extern CSigScan DebugOverlay;
    extern CSigScan TraceFunction;
    extern CSigScan CreateTrace;
    extern CSigScan GetTraceInfo;
}  // namespace signatures
