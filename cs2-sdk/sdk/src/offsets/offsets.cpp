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


    bool TraceToExit(Vector& start, Vector& direction, Vector& end, C_GameTrace* enterTrace, C_GameTrace* exitTrace, float step, float unk, void* pawn, bool* unk1)
    {
        using function_t = bool(__fastcall*)(Vector&, Vector&, Vector&, C_GameTrace*, C_GameTrace*, float, float, void*, bool*);
        static function_t fn = reinterpret_cast< function_t >(signatures::TraceToExit.GetPtrAs<void*>());

        return fn(start, direction, end, enterTrace, exitTrace, step, unk, pawn, unk1);
    }

    void ClipTraceToPlayers(Vector& start, Vector& end, C_TraceFilter* filter, C_GameTrace* trace, float unk, int mask, float unk2)
    {
        using function_t = void* (__fastcall*)(Vector&, Vector&, C_TraceFilter*, C_GameTrace*, float, int, float);
        static function_t fn = reinterpret_cast< function_t >(signatures::ClipTraceToPlayers.GetPtrAs<void*>());

        fn(start, end, filter, trace, unk, mask, unk2);
    }

    /*
    bool ISource2Client::TraceRay(const Vector_t& vecAbsStart, const Vector_t& vecAbsEnd, const CTraceFilter* pFilter, CGameTrace* pTrace)
    {
        //   @Note: this method initializes the trace filter vtable itself but copies the rest from argument
        return CallVFunc<bool, 98U>(this, &vecAbsStart, &vecAbsEnd, &pFilter[1], pTrace);
    }
    */

    void InitializeTrace(C_GameTrace* trace)
    {
        using function_t = void* (__fastcall*)(C_GameTrace*);
        static function_t fn = reinterpret_cast< function_t >(signatures::InitializeTrace.GetPtrAs<void*>());

        fn(trace);
    }

    bool HandleBulletPenetration(TraceData* trace, void* stats, UpdateValue* modValue, bool showImpacts)
    {
        using function_t = bool(__fastcall*)(TraceData*, void*, UpdateValue*, void*, void*, void*, void*, void*, bool);
        static function_t fn = reinterpret_cast< function_t >(signatures::HandleBulletPenetration.GetPtrAs<void*>());

        return fn(trace, stats, modValue, nullptr, nullptr, nullptr, nullptr, nullptr, showImpacts);
    }

    //Could call this instead of HandleBulletPen? not sure tho

    int GetContents(Vector& endPosition, int mask, int unk)
    {
        using function_t = int(__fastcall*)(void*, Vector&, int, int);
        static function_t fn = reinterpret_cast< function_t >(signatures::GetContents.GetPtrAs<void*>());

        return fn(TraceManager::Get(), endPosition, mask, unk);
    }

    void CreateTrace(TraceData* trace, Vector& start, Vector& end, C_TraceFilter* filter, int penetrationCount)
    {
        using function_t = void(__fastcall*)(TraceData*, Vector&, Vector&, C_TraceFilter*, void*, void*, void*, void*, int);
        static function_t fn = reinterpret_cast< function_t >(signatures::CreateTrace.GetPtrAs<void*>());

        fn(trace, start, end, filter, nullptr, nullptr, nullptr, nullptr, penetrationCount);
    }

    void GetTraceInfo(TraceData* trace, C_GameTrace* hit, float unknown, void* unknown2)
    {
        using function_t = void(__fastcall*)(TraceData*, C_GameTrace*, float, void*);
        static function_t fn = reinterpret_cast< function_t >(signatures::GetTraceInfo.GetPtrAs<void*>());

        return fn(trace, hit, unknown, unknown2);
    }



}
