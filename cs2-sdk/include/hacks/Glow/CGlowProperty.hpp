#pragma once

#include <type_traits>

class CGlowProperty
{
public:
    SCHEMA(bool, m_bGlowing, "CGlowProperty", "m_bGlowing");
    SCHEMA(int, m_iGlowType, "CGlowProperty", "m_iGlowType");
    SCHEMA(int, m_glowColorOverride, "CGlowProperty", "m_glowColorOverride");
};

