#pragma once
#include <input/ccsgoinput.hpp>
#include <bindings/playerpawn.hpp>
namespace misc
{
    //void BunnyHop(CCSGOInput* input);
    void BunnyHop(CUserCmd* cmd, C_CSPlayerPawnBase* pawn);
    void NoRecoil(CUserCmd* cmd, C_CSPlayerPawnBase* pawn);

    inline std::vector<Vector> bulletsPre{};
    inline std::vector<Vector> bulletsPost{};
};
