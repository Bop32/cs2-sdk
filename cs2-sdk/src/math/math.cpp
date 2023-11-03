#include "pch.hpp"

#include <math/math.hpp>

#include <imgui/imgui_internal.h>
#include <memory/memory.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <constants/constants.hpp>

#define DegToRad(x)  ((float)(x) * (float)(M_PI / 180.f))
#define RadToDegree(x)  ((float)(x) * (float)(180.f / M_PI))


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

void CMath::CorrectMovement(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove)
{
    float delta_view;
    float f1 = 0.f;
    float f2 = 0.f;

    if (old_angles.y < 0.f)
    {
        f1 = 360.0f + old_angles.y;
    }
    else
    {
        f1 = old_angles.y;
    }

    if (cmd->base->view->angles.y < 0.0f)
    {
        f2 = 360.0f + cmd->base->view->angles.y;
    }
    else
    {
        f2 = cmd->base->view->angles.y;
    }

    if (f2 < f1)
    {
        delta_view = abs(f2 - f1);
    }
    else
    {
        delta_view = 360.0f - abs(f1 - f2);
    }

    delta_view = 360.0f - delta_view;

    cmd->base->m_forwardmove = std::clamp(std::cos(DegToRad(delta_view)) * old_forwardmove + std::cos(DegToRad(delta_view + 90.f)) * old_sidemove, -450.f, 450.f);
    cmd->base->m_rightmove = std::clamp(std::sin(DegToRad(delta_view)) * old_forwardmove + std::sin(DegToRad(delta_view + 90.f)) * old_sidemove, -450.f, 450.f);
}
