#pragma once

#include <bindings/baseentity.hpp>
class C_BasePlayerWeapon : public C_BaseEntity
{
public:
    SCHEMA(int, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick")
};
