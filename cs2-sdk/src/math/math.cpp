#include "pch.hpp"

#include <math/math.hpp>

#include <imgui/imgui_internal.h>
#include <memory/memory.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <constants/constants.hpp>
#include <input/ccsgoinput.hpp>
#include <globals/globals.hpp>

#define DegToRad(x)  ((float)(x) * (float)(M_PI / 180.f))
#define RadToDegree(x)  ((float)(x) * (float)(180.f / M_PI))


using namespace globals;
void CMath::UpdateViewMatrix(VMatrix* viewMatrix)
{
    if (!viewMatrix) return;
    m_ViewMatrix = *viewMatrix;
}

void CMath::AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
    float cp = std::cos(DegToRad(angles.x)), sp = std::sin(DegToRad(angles.x));
    float cy = std::cos(DegToRad(angles.y)), sy = std::sin(DegToRad(angles.y));
    float cr = std::cos(DegToRad(angles.z)), sr = std::sin(DegToRad(angles.z));

    if (forward)
    {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    }

    if (right)
    {
        right->x = -1.f * sr * sp * cy + -1.f * cr * -sy;
        right->y = -1.f * sr * sp * sy + -1.f * cr * cy;
        right->z = -1.f * sr * cp;
    }

    if (up)
    {
        up->x = cr * sp * cy + -sr * -sy;
        up->y = cr * sp * sy + -sr * cy;
        up->z = cr * cp;
    }
}


void CMath::VectorAngles(const Vector& forward, Vector& angles, Vector* up)
{
    Vector  left;
    float   len, up_z, pitch, yaw, roll;

    len = forward.Length2D(forward);

    if (up && len > 0.001f)
    {
        pitch = RadToDegree(std::atan2(-forward.z, len));
        yaw = RadToDegree(std::atan2(forward.y, forward.x));

        // get left direction vector using cross product.
        left = (*up).Cross(forward).Normalize();

        // calculate up_z.
        up_z = (left.y * forward.x) - (left.x * forward.y);

        // calculate roll.
        roll = RadToDegree(std::atan2(left.z, up_z));
    }

    else
    {
        if (len > 0.f)
        {
            // calculate pitch and yaw.
            pitch = RadToDegree(std::atan2(-forward.z, len));
            yaw = RadToDegree(std::atan2(forward.y, forward.x));
            roll = 0.f;
        }

        else
        {
            pitch = (forward.z > 0.f) ? -90.f : 90.f;
            yaw = 0.f;
            roll = 0.f;
        }
    }

    // set out angles.
    angles = { pitch, yaw, roll };
}


void CMath::AngleVectorss(Vector angles, Vector& out)
{
    float cp = std::cos(DegToRad(angles.x)), sp = std::sin(DegToRad(angles.x));
    float cy = std::cos(DegToRad(angles.y)), sy = std::sin(DegToRad(angles.y));
    float cr = std::cos(DegToRad(angles.z)), sr = std::sin(DegToRad(angles.z));

    out.x = cp * cy;
    out.y = cp * sy;
    out.z = -sp;
}

bool CMath::WorldToScreen(const Vector& in, ImVec2& out)
{
    if (!ImGui::GetCurrentContext()) return false;

    const float z = m_ViewMatrix[3][0] * in.x + m_ViewMatrix[3][1] * in.y + m_ViewMatrix[3][2] * in.z + m_ViewMatrix[3][3];
    if (z < 0.001f) return false;

    out = ImGui::GetIO().DisplaySize * 0.5f;
    out.x *= 1.0f + (m_ViewMatrix[0][0] * in.x + m_ViewMatrix[0][1] * in.y + m_ViewMatrix[0][2] * in.z + m_ViewMatrix[0][3]) / z;
    out.y *= 1.0f - (m_ViewMatrix[1][0] * in.x + m_ViewMatrix[1][1] * in.y + m_ViewMatrix[1][2] * in.z + m_ViewMatrix[1][3]) / z;

    // Prevents rounded corners.
    out = ImFloor(out);

    return true;
}

void math::fnRandomSeed(int seed)
{
    static const auto fn = CMemory::GetProcAddress(CConstants::TIER_LIB, "RandomSeed");
    return fn.Call<decltype(&fnRandomSeed)>(seed);
}

float math::fnRandomFloat(float min, float max)
{
    static const auto fn = CMemory::GetProcAddress(CConstants::TIER_LIB, "RandomFloat");
    return fn.Call<decltype(&fnRandomFloat)>(min, max);
}

void CMath::TransformAABB(const matrix3x4_t& transform, const Vector& minsIn, const Vector& maxsIn, Vector& minsOut, Vector& maxsOut)
{
    const Vector localCenter = (minsIn + maxsIn) * 0.5f;
    const Vector localExtent = maxsIn - localCenter;

    const auto& mat = transform.m_flMatVal;
    const Vector worldAxisX { mat[0][0], mat[0][1], mat[0][2] };
    const Vector worldAxisY { mat[1][0], mat[1][1], mat[1][2] };
    const Vector worldAxisZ { mat[2][0], mat[2][1], mat[2][2] };

    const Vector worldCenter = localCenter.Transform(transform);
    const Vector worldExtent {
        localExtent.DotProductAbsolute(worldAxisX),
        localExtent.DotProductAbsolute(worldAxisY),
        localExtent.DotProductAbsolute(worldAxisZ),
    };

    minsOut = worldCenter - worldExtent;
    maxsOut = worldCenter + worldExtent;
}


Vector CMath::CalculateAngle(Vector& lookFrom, Vector& lookTo, Vector& viewAngles)
{
    Vector delta = lookFrom - lookTo;
    Vector angles;

    angles.x = RadToDegree(std::atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewAngles.x;
    angles.y = RadToDegree(std::atanf(delta.y / delta.x)) - viewAngles.y;
    angles.z = 0.0f;

    if (delta.x >= 0.0)
    {
        angles.y += 180.0f;
    }

    return angles;
}

void CMath::CorrectMovement(Vector old_angles)
{
    if (globals::localPlayerPawn->m_MoveType() == movetype_t::MOVETYPE_NOCLIP ||
        globals::localPlayerPawn->m_MoveType() == movetype_t::MOVETYPE_LADDER || 
        globals::cmd->buttons & CUserCmd::IN_USE) return;

    Vector  move, dir;
    float   delta, len;
    Vector   move_angle;

    // convert movement to vector.
    move = { cmd->base->flForwardMove, cmd->base->flSideMove, 0.f };

    // get move length and ensure we're using a unit vector ( vector with length of 1 ).
    len = move.NormalizeVecToFloat();
    if (!len)
        return;

    // convert move to an angle.
    CMath:Get().VectorAngles(move, move_angle);

    // calculate yaw delta.
    delta = (cmd->base->pViewAngles->angles.y - old_angles.y);

    // accumulate yaw delta.
    move_angle.y += delta;

    // calculate our new move direction.
    // dir = move_angle_forward * move_length
    CMath::Get().AngleVectorss(move_angle, dir);

    // scale to og movement.
    dir *= len;

    // strip old flags.
    cmd->buttons &= ~(CUserCmd::IN_FORWARD | CUserCmd::IN_BACK | CUserCmd::IN_MOVELEFT | CUserCmd::IN_MOVERIGHT);

    // fix ladder and noclip.
    if (localPlayerPawn->m_MoveType() == MOVETYPE_LADDER)
    {
        // invert directon for up and down.
        if (cmd->base->pViewAngles->angles.x >= 45.f && old_angles.x < 45.f && std::abs(delta) <= 65.f)
            dir.x = -dir.x;

        // write to movement.
        cmd->base->flForwardMove = dir.x;
        cmd->base->flSideMove = dir.y;

        // set new button flags.
        if (cmd->base->flForwardMove > 200.f)
            cmd->buttons |= CUserCmd::IN_FORWARD;

        else if (cmd->base->flForwardMove < -200.f)
            cmd->buttons |= CUserCmd::IN_BACK;

        if (cmd->base->flSideMove > 200.f)
            cmd->buttons |= CUserCmd::IN_MOVERIGHT;

        else if (cmd->base->flSideMove < -200.f)
            cmd->buttons |= CUserCmd::IN_MOVELEFT;
    }

    // we are moving normally.
    else
    {
        // we must do this for pitch angles that are out of bounds.
        if (cmd->base->pViewAngles->angles.x < -90.f || cmd->base->pViewAngles->angles.x > 90.f)
            dir.x = -dir.x;

        // set move.
        cmd->base->flForwardMove = dir.x;
        cmd->base->flSideMove = dir.y;

        // set new button flags.
        if (cmd->base->flForwardMove > 0.f)
            cmd->buttons |= CUserCmd::IN_FORWARD;

        else if (cmd->base->flForwardMove < 0.f)
            cmd->buttons |= CUserCmd::IN_BACK;

        if (cmd->base->flSideMove > 0.f)
            cmd->buttons |= CUserCmd::IN_MOVERIGHT;

        else if (cmd->base->flSideMove < 0.f)
            cmd->buttons |= CUserCmd::IN_MOVELEFT;
    }
}
