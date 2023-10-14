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
        auto GetStaticData()
        { 
            return vt::CallMethod<CEconItemDefinition*>(this, 13);
        }
    };
};
