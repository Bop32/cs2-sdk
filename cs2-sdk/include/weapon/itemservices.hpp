#pragma once

#include <schemamgr/schema_manager.hpp>

class CPlayer_ItemServices
{
public:
    SCHEMA(bool, m_bHasDefuser, "CCSPlayer_ItemServices", "m_bHasDefuser");
    SCHEMA(bool, m_bHasHelmet, "CCSPlayer_ItemServices", "m_bHasHelmet");
    SCHEMA(bool, m_bHasHeavyArmor, "CCSPlayer_ItemServices", "m_bHasHeavyArmor");
};
