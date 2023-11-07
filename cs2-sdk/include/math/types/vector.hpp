#pragma once

#include <math/types/matrix3x4.hpp>
#include <cmath>
#include <algorithm>

class Vector
{
public:
    Vector() : x(0.f), y(0.f), z(0.f) {}
    Vector(float rhs) : x(rhs), y(rhs), z(rhs) {}
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector operator+(const Vector& rhs) const { return Vector { x + rhs.x, y + rhs.y, z + rhs.z }; }
    Vector operator-(const Vector& rhs) const { return Vector { x - rhs.x, y - rhs.y, z - rhs.z }; }
    Vector operator*(float scalar) const { return Vector { x * scalar, y * scalar, z * scalar }; }
    Vector operator*=(const Vector& rhs) { return Vector { x *= rhs.x, y *= rhs.y, z *= rhs.z }; }
    Vector operator+=(const Vector& rhs) { return Vector { x += rhs.x, y += rhs.y, z += rhs.z }; }
    Vector operator-=(const Vector& rhs) { return Vector { x -= rhs.x, y -= rhs.y, z -= rhs.z }; }
    Vector operator/(const Vector& rhs) { return Vector { x / rhs.x, y / rhs.y, z / rhs.z }; }
    Vector operator/=(const Vector& rhs) { return Vector { x /= rhs.x, y /= rhs.y, z /= rhs.z }; }
    Vector operator-() { return Vector { -x, -y, -z }; }

    bool IsZero()
    {
        return x == 0 && y == 0 && z == 0;
    }

    float Length(const Vector& vector)
    {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    }

    Vector forward()
    {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length > 0.0f)
        {
            return Vector(x / length, y / length, z / length);
        }
        else
        {
            return Vector(0.0f, 0.0f, 0.0f); // Avoid division by zero
        }
    }

    Vector Normalize()
    {
        float magnitude = Length(*this);

        if (magnitude == 0.0)
        {
            return { 0.0, 0.0, 0.0 };
        }
        Vector normalized;
        normalized.x = this->x / magnitude;
        normalized.y = this->y / magnitude;
        normalized.z = this->z / magnitude;
        return normalized;
    }

    Vector Normalized()
    {
        Vector res = *this;
        float length = res.Length();

        if (length)  //-V550
            res = res / length;
        else
            res.x = res.y = res.z = 0.0f;

        return res;
    }

    float NormalizeVecToFloat()
    {
        float length = Length();

        (*this) /= (Length() + std::numeric_limits< float >::epsilon());

        return length;
    }

    float Length()
    {
        return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }


    Vector Clamp()
    {
        this->x = std::clamp(this->x, -89.0f, 89.0f);
        this->y = std::clamp(std::remainder(this->y, 360.0f), -180.0f, 180.0f);
        this->z = std::clamp(this->z, -50.0f, 50.0f);

        return Vector(x, y, z);
    }

    float Length2D()
    {
        return std::sqrt((this->x * this->x) + (this->y * this->y));
    }

    Vector Transform(const matrix3x4_t& matrix) const
    {
        return Vector { x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2] + matrix[0][3],
                      x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2] + matrix[1][3],
                      x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2] + matrix[2][3] };
    }

    float DotProductAbsolute(const Vector& rhs) const { return std::abs(x * rhs.x) + std::abs(y * rhs.y) + std::abs(z * rhs.z); }

    Vector Min(const Vector& rhs) const { return Vector { std::min(x, rhs.x), std::min(y, rhs.y), std::min(z, rhs.z) }; }
    Vector Max(const Vector& rhs) const { return Vector { std::max(x, rhs.x), std::max(y, rhs.y), std::max(z, rhs.z) }; }

    float Length2D(const Vector& rhs) const
    {
        return std::sqrt((rhs.x * rhs.x) + (rhs.y * rhs.y));
    }

    Vector Cross(const Vector& rhs)
    {
        return Vector { this->y * rhs.z - this->z * rhs.y, this->z * rhs.x - this->x * rhs.z, this->x * rhs.y - this->y * rhs.x };
    }
    float x, y, z;
};
