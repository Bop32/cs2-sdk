#pragma once

#include <virtual/virtual.hpp>


namespace material
{
    class CMaterial2
    {
    public:
        const char* GetName()
        {
            return CALL_VIRTUAL(const char*, 0, this);
        }

        const char* GetShareName()
        {
            return CALL_VIRTUAL(const char*, 1, this);
        }
    };
};
