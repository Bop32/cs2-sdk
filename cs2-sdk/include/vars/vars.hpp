#pragma once

struct Vars_t
{
    bool m_EnableESP;

    bool m_Aimbot;

    int m_AimbotFov = 1;
    bool m_SilentAim;
    bool m_AutoFire;


    bool m_PlayerBoxes;
    bool m_PlayerNames;
    bool m_PlayerHealthBar;
    bool m_Glow;

    float m_GlowColor[4] = { 40.0f / 255.0f, 48.0f / 255.0f, 108.0f / 255.0f,  255.0f / 255.0f };

    bool m_WeaponESP;
    bool m_ChickenESP;
    bool m_OtherESP;
    bool m_Use3DBoxes;

};

extern Vars_t g_Vars;
