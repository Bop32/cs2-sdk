#include <pch.hpp>
#include <stdint.h>

#include <bindings/trace.hpp>
#include <offets/offsets.hpp>

trace::C_TraceFilter::C_TraceFilter(std::uint32_t Mask,
    C_CSPlayerPawnBase* Skip1,
    C_CSPlayerPawnBase* Skip2, int Layer)
{
    TraceMask = Mask;
    V1[0] = V1[1] = 0;
    V2 = 7;
    V3 = Layer;
    V4 = 0x49;
    V5 = 0;

    SkipHandles[0] = offsets::GetEntityHandle(Skip1);
    SkipHandles[1] = Skip1->GetOwnerHandle();
    SkipHandles[2] = 0;
    SkipHandles[3] = 0;

    Collisions[0] = Skip1->GetCollisionMask();
    Collisions[1] = 0;
}

trace::C_SurfaceData* trace::C_GameTrace::GetSurfaceData()
{
    if (!offsets::GetSurfaceData) return nullptr;
    return reinterpret_cast< C_SurfaceData* >(offsets::GetSurfaceData(Surface));
}
