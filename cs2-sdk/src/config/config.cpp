#include <pch.hpp>
#include <config/config.hpp>
#include <fstream>
#include <nlohmann/json.hpp>                 
#include <logger/logger.hpp>
#include <imgui/imgui.h>

Config config;

#define PRINTER(name) printer(#name)

using namespace nlohmann;
void Config::CheckDirectory()
{
    CreateDirectory(config.directoryPath.c_str(), NULL);
}

string printer(const char* name)
{
    return std::format("{}", name);
}


void SetConfigFloatItem(float arr[], string name, json configData, int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        arr[i] = configData[name][i].get<float>();
    }
}

//For now we will only have 1 config as there are literally like 3 features
void Config::LoadConfig()
{
    auto configFilePath = config.path + "config.json";
    ifstream fileContents(configFilePath);

    //No file found so we quit
    if (!fileContents) return;


    json configData = json::parse(fileContents);
    //Not a good way for a config system, but it does the job for now.
    g_Vars.m_Aimbot = configData[PRINTER(g_Vars.m_Aimbot)].get<bool>();
    g_Vars.m_AutoFire = configData[PRINTER(g_Vars.m_AutoFire)].get<bool>();
    g_Vars.m_SilentAim = configData[PRINTER(g_Vars.m_SilentAim)].get<bool>();
    g_Vars.m_AimbotFov = configData[PRINTER(g_Vars.m_AimbotFov)].get<int>();
    g_Vars.m_EnableESP = configData[PRINTER(g_Vars.m_EnableESP)].get<bool>();
    g_Vars.m_PlayerBoxes = configData[PRINTER(g_Vars.m_PlayerBoxes)].get<bool>();
    g_Vars.m_PlayerNames = configData[PRINTER(g_Vars.m_PlayerNames)].get<bool>();
    g_Vars.m_PlayerHealthBar = configData[PRINTER(g_Vars.m_PlayerHealthBar)].get<bool>();
    g_Vars.m_Glow = configData[PRINTER(g_Vars.m_Glow)].get<bool>();
    g_Vars.m_WeaponESP = configData[PRINTER(g_Vars.m_WeaponESP)].get<bool>();
    g_Vars.m_ChickenESP = configData[PRINTER(g_Vars.m_ChickenESP)].get<bool>();
    g_Vars.m_OtherESP = configData[PRINTER(g_Vars.m_OtherESP)].get<bool>();
    g_Vars.m_Use3DBoxes = configData[PRINTER(g_Vars.m_Use3DBoxes)].get<bool>();

    SetConfigFloatItem(g_Vars.m_GlowColor, PRINTER(g_Vars.m_GlowColor), configData, IM_ARRAYSIZE(g_Vars.m_GlowColor));
    SetConfigFloatItem(g_Vars.m_PlayerInvisChamsColor, PRINTER(g_Vars.m_PlayerInvisChamsColor), configData, IM_ARRAYSIZE(g_Vars.m_PlayerInvisChamsColor));
    SetConfigFloatItem(g_Vars.m_PlayerVisibleChamsColor, PRINTER(g_Vars.m_PlayerVisibleChamsColor), configData, IM_ARRAYSIZE(g_Vars.m_PlayerVisibleChamsColor));
    SetConfigFloatItem(g_Vars.m_PlayerArmorColor, PRINTER(g_Vars.m_PlayerArmorColor), configData, IM_ARRAYSIZE(g_Vars.m_PlayerArmorColor));

    g_Vars.m_VisibleChams = configData[PRINTER(g_Vars.m_VisibleChams)].get<bool>();
    g_Vars.m_InvisibleChams = configData[PRINTER(g_Vars.m_InvisibleChams)].get<bool>();
    g_Vars.m_PlayerArmorBar = configData[PRINTER(g_Vars.m_PlayerArmorBar)].get<bool>();
    g_Vars.m_InvisibleChams = configData[PRINTER(g_Vars.m_InvisibleChams)].get<bool>();

    g_Vars.m_Bhop = configData[PRINTER(g_Vars.m_Bhop)].get<bool>();
    g_Vars.m_ViewModelFov = configData[PRINTER(g_Vars.m_ViewModelFov)].get<bool>();
    g_Vars.m_ViewModelFovSlider = configData[PRINTER(g_Vars.m_ViewModelFovSlider)].get<int>();

    g_Vars.m_WeaponName = configData[PRINTER(g_Vars.m_WeaponName)].get<bool>();

    for (auto& hitboxes : g_Vars.m_HitBoxes)
    {
        hitboxes.second = configData[PRINTER(g_Vars.m_HitBoxes)].at(hitboxes.first).get<bool>();
    }

    for (auto& chamsType : g_Vars.m_ChamsType)
    {
        chamsType.second = configData[PRINTER(g_Vars.m_ChamsType)].at(chamsType.first).get<bool>();
    }

    g_Vars.m_MinimumDamage = configData[PRINTER(g_Vars.m_MinimumDamage)].get<int>();
    g_Vars.m_SkeletonEsp = configData[PRINTER(g_Vars.m_SkeletonEsp)].get<bool>();
    g_Vars.m_RapidFire = configData[PRINTER(g_Vars.m_RapidFire)].get<bool>();

    fileContents.close();
}

void Config::SaveConfig()
{

    CheckDirectory();

    json j;

    j[PRINTER(g_Vars.m_Aimbot)] = g_Vars.m_Aimbot;
    j[PRINTER(g_Vars.m_AutoFire)] = g_Vars.m_AutoFire;
    j[PRINTER(g_Vars.m_SilentAim)] = g_Vars.m_SilentAim;
    j[PRINTER(g_Vars.m_AimbotFov)] = g_Vars.m_AimbotFov;
    j[PRINTER(g_Vars.m_EnableESP)] = g_Vars.m_EnableESP;
    j[PRINTER(g_Vars.m_PlayerBoxes)] = g_Vars.m_PlayerBoxes;
    j[PRINTER(g_Vars.m_PlayerNames)] = g_Vars.m_PlayerNames;
    j[PRINTER(g_Vars.m_PlayerHealthBar)] = g_Vars.m_PlayerHealthBar;
    j[PRINTER(g_Vars.m_Glow)] = g_Vars.m_Glow;
    j[PRINTER(g_Vars.m_GlowColor)] = g_Vars.m_GlowColor;
    j[PRINTER(g_Vars.m_PlayerInvisChamsColor)] = g_Vars.m_PlayerInvisChamsColor;
    j[PRINTER(g_Vars.m_PlayerVisibleChamsColor)] = g_Vars.m_PlayerVisibleChamsColor;
    j[PRINTER(g_Vars.m_WeaponESP)] = g_Vars.m_WeaponESP;
    j[PRINTER(g_Vars.m_ChickenESP)] = g_Vars.m_ChickenESP;
    j[PRINTER(g_Vars.m_OtherESP)] = g_Vars.m_OtherESP;
    j[PRINTER(g_Vars.m_Use3DBoxes)] = g_Vars.m_Use3DBoxes;
    j[PRINTER(g_Vars.m_VisibleChams)] = g_Vars.m_VisibleChams;
    j[PRINTER(g_Vars.m_InvisibleChams)] = g_Vars.m_InvisibleChams;
    j[PRINTER(g_Vars.m_PlayerArmorBar)] = g_Vars.m_PlayerArmorBar;
    j[PRINTER(g_Vars.m_PlayerArmorColor)] = g_Vars.m_PlayerArmorColor;
    j[PRINTER(g_Vars.m_Bhop)] = g_Vars.m_Bhop;
    j[PRINTER(g_Vars.m_ViewModelFov)] = g_Vars.m_ViewModelFov;
    j[PRINTER(g_Vars.m_ViewModelFovSlider)] = g_Vars.m_ViewModelFovSlider;
    j[PRINTER(g_Vars.m_WeaponName)] = g_Vars.m_WeaponName;
    j[PRINTER(g_Vars.m_HitBoxes)] = g_Vars.m_HitBoxes;
    j[PRINTER(g_Vars.m_MinimumDamage)] = g_Vars.m_MinimumDamage;
    j[PRINTER(g_Vars.m_ChamsType)] = g_Vars.m_ChamsType;
    j[PRINTER(g_Vars.m_SkeletonEsp)] = g_Vars.m_SkeletonEsp;
    j[PRINTER(g_Vars.m_RapidFire)] = g_Vars.m_RapidFire;



    fstream configFile(config.path, fstream::in | fstream::out | fstream::trunc);

    if (!configFile)
    {
        configFile.open(config.path + "config.json", fstream::in | fstream::out | fstream::trunc);
        configFile << std::setw(4) << j;
        //configFile << PRINTER(g_Vars.m_Aimbot);
        configFile.close();
    }

}

