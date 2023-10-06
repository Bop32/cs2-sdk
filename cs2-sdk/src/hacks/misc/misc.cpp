#include "pch.hpp"
#include <hacks/misc/misc.hpp>
#include <bindings/playercontroller.hpp>
#include <bindings/playerpawn.hpp>
#include <offets/offsets.hpp>
#include <interfaces/gameentitysystem.hpp>

void misc::BunnyHop(CCSGOInput* input)
{
    auto localPlayerController = CGameEntitySystem::GetLocalPlayerController();

    C_CSPlayerPawnBase* pawn = localPlayerController->m_hPawn().Get();

    bool jumped = false;
    if (GetAsyncKeyState(VK_SPACE) < 0)
    {
        if (pawn->m_iFlags() & flags_t::FL_ONGROUND)
        {
                {
                offsets::SetForceJump(65537);
                jumped = true;
            }
        }
        else if(jumped)
        {
            offsets::SetForceJump(256);
            jumped = false;
        }
    }
}
