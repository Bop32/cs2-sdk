#pragma once

#include <bindings/baseentity.hpp>
#include <weapon/c_attributecontainer.hpp>
class C_BasePlayerWeapon : public C_BaseEntity
{
public:
    SCHEMA(int, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick")
    PSCHEMA(C_AttributeContainer, m_AttributeManager, "C_EconEntity", "m_AttributeManager")
    SCHEMA(int32_t, m_nFallbackPaintKit, "C_EconEntity", "m_nFallbackPaintKit")
    SCHEMA(int32_t, m_nFallbackStatTrak, "C_EconEntity", "m_nFallbackStatTrak")
    SCHEMA(int32_t, m_nFallbackSeed, "C_EconEntity", "m_nFallbackSeed")
};
