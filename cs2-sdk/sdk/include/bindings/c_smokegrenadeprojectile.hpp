#pragma once

#include <bindings/baseentity.hpp>
class C_SmokeGrenadeProjectile : public C_BaseEntity
{
public:
    SCHEMA(bool, m_bDidSmokeEffect, "C_SmokeGrenadeProjectile", "m_bDidSmokeEffect");
};
