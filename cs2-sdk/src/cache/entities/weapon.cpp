#include "pch.hpp"

#include <cache/entities/weapon.hpp>

#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

#include <imgui/imgui.h>

CCachedWeapon::CCachedWeapon(CBaseHandle handle) : CCachedBaseEntity(handle) { m_BoxColor = IM_COL32(255, 255, 0, 255); }

void RenderGlow(C_BaseEntity* entity, bool on)
{
    entity->m_Glow().m_bGlowing() = on;
    entity->m_Glow().m_iGlowType() = 3;
    entity->m_Glow().m_glowColorOverride() = IM_COL32(g_Vars.m_GlowColor[0] * 255,
        g_Vars.m_GlowColor[1] * 255, g_Vars.m_GlowColor[2] * 255, g_Vars.m_GlowColor[3] * 255);
}


bool CCachedWeapon::CanDrawESP()
{
    C_BaseEntity* weapon = Get<C_BaseEntity>();

    if (!g_Vars.m_WeaponESP)
    {
        weapon->m_Glow().m_bGlowing() = false;
        return false;
    }


    if (!weapon || weapon->m_hOwnerEntity().IsValid())
    {
        return false;
    }

    return true;
}

void CCachedWeapon::RenderESP()
{
    C_BaseEntity* weapon = Get<C_BaseEntity>();
    RenderGlow(weapon, true);
}

