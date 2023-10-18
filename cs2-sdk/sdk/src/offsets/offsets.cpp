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

    void ClipTraceToPlayers(Vector& start, Vector& end, trace::C_TraceFilter* filter, trace::C_GameTrace* trace, float unk, float mask, float unk2)
    {
        using function_t = void* (__fastcall*)(Vector&, Vector&, trace::C_TraceFilter*, trace::C_GameTrace*, float, float, float);
        static function_t fn = reinterpret_cast< function_t >(signatures::ClipTraceToPlayers.GetPtrAs<void*>());

        fn(start, end, filter, trace, unk, mask, unk2);
    }


    bool HandleBulletPenetration(void* ray, void* trace, weapon::CCSWeaponBaseVData* weaponData, int mask, int& showImpacts)
    {
        using function_t = bool(__fastcall*)(void*, void*, weapon::CCSWeaponBaseVData*, int, int&);
        static function_t fn = reinterpret_cast< function_t >(signatures::HandleBulletPenetration.GetPtrAs<void*>());

        return fn(ray, trace, weaponData, mask ,showImpacts);
    }
    /*
    bool HandleBulletPenetration(float& weaponPenetration, int& material, bool& hitGrate, void* trace, Vector& direction, void* traceSurface, float penetration, float surfaceDamageModifer, bool unk,uint32_t mask, float weaponPenetrationBefore, int& penetrationCount, Vector& startPosition, float range, float &traceLength, uint32_t playerTeamNum, void* localPlayer)
    {
        using function_t = bool(__fastcall*)(float&, int&, bool&, void*, Vector&, void*, float, float, bool, uint32_t, float, int&, float, float&, uint32_t, void*);
        static function_t fn = reinterpret_cast< function_t >(signatures::HandleBulletPenetration.GetPtrAs<void*>());

        return fn(weaponPenetration, material, hitGrate,
            trace, direction, traceSurface,
            penetration, surfaceDamageModifer, unk,
            mask, weaponPenetrationBefore, penetrationCount,
            range, traceLength,
            playerTeamNum, localPlayer);
    }
    */
}
