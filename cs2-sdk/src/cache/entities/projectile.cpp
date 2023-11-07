#include <pch.hpp>
#include <cache/entities/projecitle.hpp>
#include <bindings/baseentity.hpp>
#include <logger/logger.hpp>
#include <interfaces/schemasystem.hpp>
#include <bindings/c_smokegrenadeprojectile.hpp>

bool CCachedProjectile::CanDrawESP()
{
    C_BaseEntity* projectile = Get<C_BaseEntity>();

    if (!projectile || !projectile->IsProjectile())
    {
        return false;
    }

    return true;
}

void CCachedProjectile::RenderESP()
{
    C_BaseEntity* projectile = Get<C_BaseEntity>();

    CSchemaClassInfo* dynamicBinding = projectile->Schema_DynamicBinding();

    if (!dynamicBinding) return;

    if (strstr(dynamicBinding->m_Name, "SmokeGrenade"))
    {
        auto smoke = reinterpret_cast< C_SmokeGrenadeProjectile*>(projectile);
        smoke->m_bDidSmokeEffect() = true;
    }
}
