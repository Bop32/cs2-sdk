#pragma once

#include <bindings/playerpawn.hpp>
namespace AutoWall
{
    void ScaleDamage(C_CSPlayerPawnBase* pawn, weapon::CCSWeaponBaseVData* weaponData, float& damage, int hitgroup);
    bool CanHit(C_CSPlayerPawnBase* player, Vector localPlayerPosition, Vector shootPosition, weapon::CCSWeaponBaseVData* weaponInfo, float& damage);
};
