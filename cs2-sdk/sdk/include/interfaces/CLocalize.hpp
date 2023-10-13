#pragma once

#include <virtual/virtual.hpp>

class CLocalize
{
public:
    static CLocalize* Get();

    auto FindSafe(const char* tokenName)
    {
        return CALL_VIRTUAL(const char*, 17, this, tokenName);
    }
};
