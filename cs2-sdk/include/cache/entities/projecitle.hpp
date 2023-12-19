#pragma once

#include <cache/entities/base.hpp>

// Has been named "Gun" to possibly avoid future naming conflicts.
// CCachedGun can also be a knife/taser/etc.

class CCachedProjectile : public CCachedBaseEntity
{
public:
    CCachedProjectile() { m_BoxColor = IM_COL32(255, 255, 0, 255); }

    Type GetType() const override { return Type::PROJECTILE; }

    bool CanDrawESP() override;

    void RenderESP() override;
};