#include <pch.hpp>
#include <hacks/anti_aim/anti_aim.hpp>
#include <globals/globals.hpp>
#include <bindings/playerpawn.hpp>

using namespace globals;
void AntiAim::RunAntiAim(CUserCmd* cmd, CCSGOInput* input)
{
    if(localPlayerPawn->m_MoveType() == movetype_t::MOVETYPE_NOCLIP || 
        localPlayerPawn->m_MoveType() == movetype_t::MOVETYPE_LADDER || cmd->buttons & CUserCmd::IN_USE) return;

    Vector currentViewAngles;
    CCSGOInput::Get()->GetViewAngles(currentViewAngles);
    float yaw_base = currentViewAngles.y;
    float x = math::fnRandomFloat(89.0f, 0);


    Vector angles = Vector(x, yaw_base + 180.f, 0.f);

    cmd->base->pViewAngles->angles = angles;
}
