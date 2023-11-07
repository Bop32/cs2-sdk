#include "pch.hpp"

#include <cache/entities/weapon.hpp>

#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

#include <imgui/imgui.h>

#include <interfaces/schemasystem.hpp>
#include <logger/logger.hpp>

void RenderGlow(C_BaseEntity* entity, bool on)
{
    entity->m_Glow().m_bGlowing() = on;
    entity->m_Glow().m_iGlowType() = 3;
    entity->m_Glow().m_glowColorOverride() = IM_COL32(g_Vars.m_GlowColor[0] * 255,
        g_Vars.m_GlowColor[1] * 255, g_Vars.m_GlowColor[2] * 255, g_Vars.m_GlowColor[3] * 255);
}

bool CCachedGun::CanDrawESP()
{
    if (!g_Vars.m_WeaponESP)
    {
        return false;
    }

    C_BaseEntity* weapon = Get<C_BaseEntity>();
    if (!weapon || weapon->m_hOwnerEntity().IsValid())
    {
        return false;
    }

    return true;
}

void CCachedGun::RenderESP()
{
   return;
}

