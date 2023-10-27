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
    void ClipTraceToPlayers(Vector& start, Vector& end, C_TraceFilter* filter, C_GameTrace* trace, float unk, int mask, float unk2);
    bool TraceToExit(Vector& start, Vector& direction, Vector& end, C_GameTrace* enterTrace, C_GameTrace* exitTrace, float step, float unk, void* pawn, bool* unk1);
    void InitializeTrace(C_GameTrace* trace);
    bool HandleBulletPenetration(TraceData* trace, void* stats, UpdateValue* modValue, bool showImpacts = false);
    int GetContents(Vector& endPosition, int mask, int unk);
    void CreateTrace(TraceData* trace, Vector& start, Vector& end, C_TraceFilter* filter, int penetrationCount);
    void GetTraceInfo(TraceData* trace, C_GameTrace* hit, float unknown, void* unknown2);
}
