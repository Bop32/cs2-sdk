#include "pch.hpp"
#include <hacks/misc/misc.hpp>
#include <bindings/playercontroller.hpp>
#include <bindings/playerpawn.hpp>
#include <offets/offsets.hpp>
#include <interfaces/gameentitysystem.hpp>

static bool onGround = false;
void misc::BunnyHop(CUserCmd* cmd, C_CSPlayerPawnBase* pawn)
{
    /*
    if (!(pawn->m_iFlags() & FL_ONGROUND) && cmd->buttons & CUserCmd::IN_JUMP)
    {
        cmd->buttons &= ~CUserCmd::IN_JUMP;
    }
    */

    if (static bool bShouldFakeJump = false; bShouldFakeJump)
    {
        cmd->buttons |= CUserCmd::IN_JUMP;
        bShouldFakeJump = false;
    }
    // check is player want to jump
    else if (cmd->buttons & CUserCmd::IN_JUMP)
    {
        // check is player on the ground
        if (pawn->m_iFlags() & FL_ONGROUND)
        {
            bShouldFakeJump = true;
        }
        else
        {
            cmd->buttons &= ~CUserCmd::IN_JUMP;
        }
    }
}
