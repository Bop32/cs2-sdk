#pragma once
#include <virtual/virtual.hpp>
#include <weapon/ceconitemschema.hpp>

class CSource2Client {
   public:
    static CSource2Client* Get();

    auto GetEconItemSystem()
    {
        return 0;
        //return CALL_VIRTUAL(CEconItemSystem*, 111, this);
    }
};
