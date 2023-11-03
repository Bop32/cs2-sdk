#pragma once

#include <math/types/transform.hpp>
#include <math/types/vmatrix.hpp>
#include <input/cusercmd.hpp>
struct ImVec2;

class CMath
{
public:
    static CMath& Get()
    {
        static CMath inst;
        return inst;
    }

    void UpdateViewMatrix(VMatrix* viewMatrix);

    void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);

    void VectorAngles(const Vector& forward, Vector& angles, Vector* up = nullptr);

    void AngleVectorss(Vector angles, Vector& out);

    bool WorldToScreen(const Vector& in, ImVec2& out);
    void TransformAABB(const matrix3x4_t& transform, const Vector& minsIn, const Vector& maxsIn, Vector& minsOut, Vector& maxsOut);
    Vector CalculateAngle(Vector& lookFrom, Vector& lookTo, Vector& viewAngles);
    void CorrectMovement(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove);


private:
    VMatrix m_ViewMatrix;
};

namespace math
{
    void fnRandomSeed(int seed);
    float fnRandomFloat(float min, float max);
};
