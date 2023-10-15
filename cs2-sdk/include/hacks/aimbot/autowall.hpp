#pragma once

#include <bindings/playerpawn.hpp>
namespace AutoWall
{
    void ScaleDamage(C_CSPlayerPawnBase* pawn, weapon::CCSWeaponBaseVData* weaponData, float& damage, int hitgroup);
    bool CanHit(C_CSPlayerPawnBase* player, weapon::CCSWeaponBaseVData* weaponInfo, int hitgroup, float fraction, float& damage);
};
