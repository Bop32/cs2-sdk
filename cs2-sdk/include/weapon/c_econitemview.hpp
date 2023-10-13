#pragma once

#include <schemamgr/schema_manager.hpp>
#include <cstdint>
#include <weapon/ceconitemdefinition.hpp>
#include <virtual/virtual.hpp>

namespace weapon
{
    class CEconItem;

    class C_EconItemView
    {
    public:
        CEconItemDefinition* GetStaticData()
        { 
            return vt::CallMethod<CEconItemDefinition*>(this, 13);
        }

        SCHEMA(uint16_t, m_iItemDefinitionIndex, "C_EconItemView", "m_iItemDefinitionIndex");
        SCHEMA(uint64_t, m_iItemID, "C_EconItemView", "m_iItemID");
        SCHEMA(uint32_t, m_iItemIDLow, "C_EconItemView", "m_iItemIDLow");
        SCHEMA(uint32_t, m_iItemIDHigh, "C_EconItemView", "m_iItemIDHigh");
        SCHEMA(uint32_t, m_iAccountID, "C_EconItemView", "m_iAccountID");
    };
};
