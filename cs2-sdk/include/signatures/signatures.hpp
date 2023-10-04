#pragma once

#include <sigscan/sigscan.hpp>

namespace signatures {
    extern CSigScan GetBaseEntity, GetHighestEntityIndex;
    extern CSigScan GetLocalPlayer;
    extern CSigScan GetHandleFromEntity;
    extern CSigScan GetCSGOInput;
    extern CSigScan GetIsDemoOrHLTV;
    extern CSigScan GetMatricesForView;
    extern CSigScan GetHitboxSet;
    extern CSigScan HitboxToWorldTransforms;
    extern CSigScan GetForceJump;
    extern CSigScan GetBonePosition;
    extern CSigScan SetViewAngles;
    extern CSigScan GetViewAngles;
    extern CSigScan MaxBoneCount;
}  // namespace signatures
