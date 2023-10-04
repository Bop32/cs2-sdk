#include "pch.hpp"
#include <hacks/misc/misc.hpp>
#include <bindings/playercontroller.hpp>
#include <bindings/playerpawn.hpp>
#include <offets/offsets.hpp>
#include <interfaces/gameentitysystem.hpp>

void misc::BunnyHop(CUserCmd* cmd)
{
    auto localPlayerController = CGameEntitySystem::GetLocalPlayerController();

    C_CSPlayerPawnBase* pawn = localPlayerController->m_hPawn().Get();

    if (pawn->m_iFlags() & flags_t::FL_ONGROUND)
    {
        //offsets::SetForceJump(65537);
    }
    else
    {
        //offsets::SetForceJump(256);
    }
}
