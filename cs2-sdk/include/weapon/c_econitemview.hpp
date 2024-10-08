#pragma once

#include <schemamgr/schema_manager.hpp>
#include <cstdint>
#include <weapon/ceconitemdefinition.hpp>
#include <virtual/virtual.hpp>
#include <signatures/signatures.hpp>

class CCSWeaponBaseVData
{
public:
    SCHEMA(int32_t, m_nDamage, "CCSWeaponBaseVData", "m_nDamage");  // Offset: 3376
    SCHEMA(float, m_flHeadshotMultiplier, "CCSWeaponBaseVData", "m_flHeadshotMultiplier");  // Offset: 3380
    SCHEMA(float, m_flArmorRatio, "CCSWeaponBaseVData", "m_flArmorRatio");  // Offset: 3384
    SCHEMA(float, m_flPenetration, "CCSWeaponBaseVData", "m_flPenetration");  // Offset: 3388
    SCHEMA(float, m_flRange, "CCSWeaponBaseVData", "m_flRange"); // Offset: 3392
    SCHEMA(float, m_flRangeModifier, "CCSWeaponBaseVData", "m_flRangeModifier");  // Offset: 3396
    SCHEMA(int32_t, m_nNumBullets, "CBasePlayerWeaponVData", "m_nNumBullets");
    SCHEMA(int32_t, m_iWeight, "CBasePlayerWeaponVData", "m_iWeight");
    SCHEMA(float, m_flMaxSpeed, "CCSWeaponBaseVData", "m_flMaxSpeed");
    SCHEMA(int32_t, m_nZoomLevels, "CCSWeaponBaseVData", "m_nZoomLevels ");
};


class CEconItem;

class C_EconItemView
{
public:
    CCSWeaponBaseVData* GetWeaponInfo();
    auto GetStaticData()
    {
        return vt::CallMethod<CEconItemDefinition*>(this, 13);
    }
    SCHEMA(uint64_t, m_iItemID, "C_EconItemView", "m_iItemID");
    SCHEMA(uint16_t, m_iItemDefinitionIndex, "C_EconItemView", "m_iItemDefinitionIndex");

};
