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
        using function_t = bool(__fastcall*)(void*, C_Ray*, Vector&, Vector&, void*, C_GameTrace*);
        static function_t fn = reinterpret_cast< function_t >(signatures::TraceShape.GetPtrAs<void*>());

        return fn(TraceManager::Get(), Ray, Start, End, Filter, TraceResult);
    }


    bool TraceToExit(Vector& start, Vector& direction, Vector& end, trace::C_GameTrace* enterTrace, trace::C_GameTrace* exitTrace, float step, float unk, void* pawn, bool* unk1)
    {
        using function_t = bool(__fastcall*)(Vector&, Vector&, Vector&, trace::C_GameTrace*, trace::C_GameTrace*, float, float, void*, bool*);
        static function_t fn = reinterpret_cast< function_t >(signatures::TraceToExit.GetPtrAs<void*>());

        return fn(start, direction, end, enterTrace, exitTrace, step, unk, pawn, unk1);
    }

    void ClipTraceToPlayers(Vector& start, Vector& end, trace::C_TraceFilter* filter, trace::C_GameTrace* trace, float unk, float mask, float unk2)
    {
        using function_t = void* (__fastcall*)(Vector&, Vector&, trace::C_TraceFilter*, trace::C_GameTrace*, float, float, float);
        static function_t fn = reinterpret_cast< function_t >(signatures::ClipTraceToPlayers.GetPtrAs<void*>());

        fn(start, end, filter, trace, unk, mask, unk2);
    }


    bool HandleBulletPenetration(void* ray, void* trace, CCSWeaponBaseVData* weaponData, int mask, int& showImpacts)
    {
        using function_t = bool(__fastcall*)(void*, void*, CCSWeaponBaseVData*, int, int&);
        static function_t fn = reinterpret_cast< function_t >(signatures::HandleBulletPenetration.GetPtrAs<void*>());

        return fn(ray, trace, weaponData, mask, showImpacts);
    }

    //Could call this instead of HandleBulletPen? not sure tho
    bool GetDamageToPoint(void* ray, float a2, float a3, float a4, unsigned int a5, int a6, int* a7)
    {
        using function_t = bool(__fastcall*)(void*, float, float, float, unsigned int, int, int*);
        static function_t fn = reinterpret_cast< function_t >(signatures::GetDamageToPoint.GetPtrAs<void*>());

        return fn(ray, a2, a3, a4, a5, a6, a7);
    }
}
