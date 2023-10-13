#pragma once
#include <math/types/vector.hpp>

class CEngineClient {
   public:
    static CEngineClient* Get();

    bool IsInGame();
    int GetLocalPlayer();
    void GetScreenSize(int& w, int& h);
};
