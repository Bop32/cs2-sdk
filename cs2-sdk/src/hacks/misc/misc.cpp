#include "pch.hpp"
#include <hacks/misc/misc.hpp>
#include <bindings/playercontroller.hpp>
#include <bindings/playerpawn.hpp>
#include <offets/offsets.hpp>
#include <interfaces/gameentitysystem.hpp>

static auto bWasLastTimeOnGround = false;

void misc::BunnyHop(CUserCmd* cmd, C_CSPlayerPawnBase* pawn)
{
    if (!(pawn->m_iFlags() & 1) != 0)
    {
        cmd->buttons &= ~CUserCmd::IN_JUMP;
    }
}
