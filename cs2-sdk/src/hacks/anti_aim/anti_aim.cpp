#include <pch.hpp>
#include <hacks/anti_aim/anti_aim.hpp>
#include <globals/globals.hpp>
#include <bindings/playerpawn.hpp>

using namespace globals;
void AntiAim::RunAntiAim(CUserCmd* cmd, CCSGOInput* input)
{
    if(localPlayerPawn->m_MoveType() == movetype_t::MOVETYPE_NOCLIP || 
        localPlayerPawn->m_MoveType() == movetype_t::MOVETYPE_LADDER) return;

    Vector currentViewAngles;
    CCSGOInput::Get()->GetViewAngles(currentViewAngles);
    float yaw_base = currentViewAngles.y;

    Vector angles = Vector(89.f, yaw_base + 180.f, 0.f);

    cmd->base->view->angles = angles;
}
