#include <pch.hpp>
#include <offets/offsets.hpp>
#include <bindings/trace.hpp>
#include <math/types/vector.hpp>

using namespace trace;

namespace offsets
{
    uint32_t GetEntityHandle(void* pawn)
    {
        return signatures::GetEntityHandle.GetPtr().Call<uint32_t(*)(void*)>(pawn);
    }

    uint64_t GetSurfaceData(void* surface)
    {
        return signatures::GetSurfaceData.GetPtr().Call<uint64_t(*)(void*)>(surface);
    }                                   

    bool TraceShape(C_Ray* Ray, Vector& Start, Vector& End, void* Filter, C_GameTrace* TraceResult)
    {
        using function_t = bool(__fastcall*)(void*, C_Ray*, Vector*, Vector*, void*, C_GameTrace*);
        static function_t fn = reinterpret_cast< function_t >(signatures::TraceShape.GetPtrAs<void*>());

        return fn(TraceManager::Get(), Ray, &Start, &End, Filter, TraceResult);
    }

    void SetForceJump(uint32_t value)
    {
        auto forceAttack = signatures::GetForceJump.GetPtrAs<uint32_t*>();
        *forceAttack = value;
    }

}
