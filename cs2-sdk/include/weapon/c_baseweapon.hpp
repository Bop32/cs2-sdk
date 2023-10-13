#pragma once

#include <bindings/baseentity.hpp>
#include <weapon/c_attributecontainer.hpp>
class C_BasePlayerWeapon : public C_BaseEntity
{
public:
    SCHEMA(int, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick")
    SCHEMA(C_AttributeContainer*, m_AttributeManager, "C_EconEntity", "m_AttributeManager")

};
