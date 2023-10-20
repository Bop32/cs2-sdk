#pragma once

#include <bindings/playerpawn.hpp>
#include <bindings/trace.hpp>

using namespace trace;
namespace AutoWall
{
    struct FireBulletData
    {
        FireBulletData(const Vector& eye_pos)
            : src(eye_pos)
        {
        }

        Vector          src;
        C_GameTrace     enterTrace;
        Vector          direction;
        C_TraceFilter   filter;
        float           traceLength;
        float           traceLengthRemaining;
        float           currentDamage;
        int             penetrateCount;
    };

    void ScaleDamage(C_CSPlayerPawnBase* pawn, CCSWeaponBaseVData* weaponData, float& damage, int hitgroup);
    bool CanHit(C_CSPlayerPawnBase* player, Vector localPlayerPosition, Vector shootPosition, CCSWeaponBaseVData* weaponInfo, float& damage);
    bool SimulateFireBullet(CCSWeaponBaseVData* weaponData, FireBulletData& data, C_CSPlayerPawnBase* enemy);
};
