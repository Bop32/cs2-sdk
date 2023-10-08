#include <pch.hpp>
#include <config/config.hpp>
#include <fstream>
#include <nlohmann/json.hpp>                 
#include <logger/logger.hpp>

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
    g_Vars.m_GlowColor[0] = configData[PRINTER(g_Vars.m_GlowColor)][0].get<float>();
    g_Vars.m_GlowColor[1] = configData[PRINTER(g_Vars.m_GlowColor)][1].get<float>();
    g_Vars.m_GlowColor[2] = configData[PRINTER(g_Vars.m_GlowColor)][2].get<float>();
    g_Vars.m_GlowColor[3] = configData[PRINTER(g_Vars.m_GlowColor)][3].get<float>();
    g_Vars.m_WeaponESP = configData[PRINTER(g_Vars.m_WeaponESP)].get<bool>();
    g_Vars.m_ChickenESP = configData[PRINTER(g_Vars.m_ChickenESP)].get<bool>();
    g_Vars.m_OtherESP = configData[PRINTER(g_Vars.m_OtherESP)].get<bool>();
    g_Vars.m_Use3DBoxes = configData[PRINTER(g_Vars.m_Use3DBoxes)].get<bool>();

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
    j[PRINTER(g_Vars.m_WeaponESP)] = g_Vars.m_WeaponESP;
    j[PRINTER(g_Vars.m_ChickenESP)] = g_Vars.m_ChickenESP;
    j[PRINTER(g_Vars.m_OtherESP)] = g_Vars.m_OtherESP;
    j[PRINTER(g_Vars.m_Use3DBoxes)] = g_Vars.m_Use3DBoxes;

    fstream configFile(config.path, fstream::in | fstream::out | fstream::trunc);

    if (!configFile)
    {
        configFile.open(config.path + "config.json", fstream::in | fstream::out | fstream::trunc);
        configFile << std::setw(4) << j;
        //configFile << PRINTER(g_Vars.m_Aimbot);
        configFile.close();
    }

}

