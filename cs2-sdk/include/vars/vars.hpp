#pragma once
#include <map>

struct Vars_t
{
    bool m_EnableESP;

    bool m_Aimbot;

    int m_AimbotFov = 1;
    bool m_SilentAim;
    bool m_AutoFire;


    bool m_PlayerBoxes;
    bool m_PlayerNames;
    bool m_WeaponName;
    bool m_PlayerHealthBar;
    bool m_SkeletonEsp;
    bool m_PlayerArmorBar;
    bool m_PlayerHasDefuser;
    bool m_Glow;

    std::map<const char*, bool> m_HitBoxes = 
    {
        {"Head", false}, 
        {"Chest", false}, 
        {"Stomach", false}, 
        {"Arms", false}, 
        {"Legs", false}
    };

    std::map<const char*, bool> m_ChamsType =
    {
        {"Texture", true},
        {"Flat", false},
    };

    float m_GlowColor[4] = { 180.0f / 255.0f, 60.0f / 255.0f, 120.0f / 255.0f,  153.0f / 255.0f };
    float m_PlayerInvisChamsColor[4] = { 150.0f / 255.0f, 200.0f / 255.0f, 60.0f / 255.0f,  255.0f / 255.0f };
    float m_PlayerVisibleChamsColor[4] = { 150.0f / 255.0f, 200.0f / 255.0f, 60.0f / 255.0f,  255.0f / 255.0f };
    float m_PlayerArmorColor[4] = { 102.f / 255.f, 190.f / 255.f, 208.f / 255.f, 255.f / 255.f };

    int m_MinimumDamage = 50;

    bool m_VisibleChams;
    bool m_InvisibleChams;

    bool m_WeaponESP;
    bool m_ChickenESP;
    bool m_OtherESP;
    bool m_Use3DBoxes;

    bool m_Bhop;
    bool m_ViewModelFov;
    int  m_ViewModelFovSlider = 80;

};

extern Vars_t g_Vars;
