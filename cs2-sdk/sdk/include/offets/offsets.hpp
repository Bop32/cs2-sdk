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
    void ClipTraceToPlayers(Vector& start, Vector& end, trace::C_TraceFilter* filter, trace::C_GameTrace* trace, float unk, float mask, float unk2);
    bool TraceToExit(Vector& start, Vector& direction, Vector& end, trace::C_GameTrace* enterTrace, trace::C_GameTrace* exitTrace, float step, float unk, void* pawn, bool* unk1);
    void TraceRay(C_Ray* ray, C_GameTrace* trace, C_TraceFilter* filter, float unk, void* unk1);
    /*
    bool HandleBulletPenetration(
        float& weaponPenetration, int& material, bool& hitGrate, 
        void* trace, Vector& direction, void* traceSurface, 
        float penetration, float surfaceDamageModifer, bool unk,
        uint32_t mask, float weaponPenetrationBefore, int& penetrationCount, 
        Vector& startPosition, float range, float& traceLength, 
        uint32_t playerTeamNum, void* localPlayer);
    */
    bool HandleBulletPenetration(void* ray, void* trace, CCSWeaponBaseVData* weaponData, int mask, int& showImpacts);

    int GetContents(Vector& endPosition, int mask, int unk);
}
