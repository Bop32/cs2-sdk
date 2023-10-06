#include <signatures/signatures.hpp>
#include <cstdint>
#include <module/module.hpp>
#include <math/types/vector.hpp>
#include <bindings/trace.hpp>

using namespace trace;
namespace offsets
{
    uint32_t GetEntityHandle(void*);
    uint64_t GetSurfaceData(void* surface);
    bool TraceShape(C_Ray* Ray, Vector& Start, Vector& End, void* Filter, C_GameTrace* TraceResult);
    void SetForceJump(uint32_t value);
}
