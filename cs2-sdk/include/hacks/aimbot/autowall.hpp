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

    struct WeaponData
    {
        WeaponData(const float weaponDamage, const float weaponPenetration, const float weaponRange, const float weaponRangeModifer,
            const int penetrationCount, const bool failed) :
            weaponDamage(weaponDamage),
            weaponPenetration(weaponPenetration),
            weaponRange(weaponRange),
            weaponRangeModifer(weaponRangeModifer),
            penetrationCount(penetrationCount),
            failed(failed)
        {
        }

        float weaponDamage { }, weaponPenetration { }, weaponRange { }, weaponRangeModifer { };
        int penetrationCount { };
        bool failed { };
    };

    void ScaleDamage(C_CSPlayerPawnBase* pawn, CCSWeaponBaseVData* weaponData, float& damage, int hitgroup, bool hasArmor);
    bool CanHit(C_CSPlayerPawnBase* player, Vector localPlayerPosition, Vector shootPosition, CCSWeaponBaseVData* weaponInfo, float& damage);
    bool SimulateFireBullet(CCSWeaponBaseVData* weaponData, FireBulletData& data, C_CSPlayerPawnBase* enemy);
};
