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
}  // namespace signatures
