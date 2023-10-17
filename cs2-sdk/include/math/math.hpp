#pragma once

#include <math/types/transform.hpp>
#include <math/types/vmatrix.hpp>

struct ImVec2;

class CMath {
   public:
    static CMath& Get() {
        static CMath inst;
        return inst;
    }

    void UpdateViewMatrix(VMatrix* viewMatrix);

    void AngleVectors(Vector angles, Vector& out);

    bool WorldToScreen(const Vector& in, ImVec2& out);
    void TransformAABB(const matrix3x4_t& transform, const Vector& minsIn, const Vector& maxsIn, Vector& minsOut, Vector& maxsOut);
    Vector CalculateAngle(Vector& lookFrom, Vector& lookTo, Vector& viewAngles);
   private:
    VMatrix m_ViewMatrix;
};
