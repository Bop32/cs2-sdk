#include "pch.hpp"
#include <hacks/misc/misc.hpp>
#include <bindings/playercontroller.hpp>
#include <bindings/playerpawn.hpp>
#include <offets/offsets.hpp>
#include <interfaces/gameentitysystem.hpp>

void misc::BunnyHop(CUserCmd* cmd, C_CSPlayerPawnBase* pawn)
{
    if (!(pawn->m_iFlags() & 1) != 0)
    {
        cmd->buttons &= ~CUserCmd::IN_JUMP;
    }
}

void misc::NoRecoil(CUserCmd* cmd, C_CSPlayerPawnBase* pawn)
{
    auto aimPunch = pawn->m_aimPunchCache();

    if (aimPunch.m_Size > 0 && aimPunch.m_Size < 0xFFFF)
    {
        auto recoil = aimPunch.m_Data[aimPunch.m_Size - 1];

        static Vector prev = Vector(0.f, 0.f, 0.f);

        auto delta = prev - recoil;

        bulletsPre.push_back(delta);

        cmd->base->view->angles += delta * 2;

        cmd->base->view->angles.Clamp(); // Remove?

        CCSGOInput::Get()->SetViewAngles(cmd->base->view->angles);
 
        bulletsPost.push_back(cmd->base->view->angles);

        prev = recoil;
    } else {
        bulletsPre.clear();
        bulletsPost.clear();
    }
}
