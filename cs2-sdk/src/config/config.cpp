#include <pch.hpp>
#include <config/config.hpp>
#include <fstream>

Config config;

#define PRINTER(name) printer(#name, (name))

void Config::CheckDirectory()
{
    CreateDirectory(config.directoryPath.c_str(), NULL);
}

string printer(const char* name, int value)
{
    return std::format("{};{};", name, value);
}

void Config::SaveConfig()
{
    CheckDirectory();

    fstream configFile(config.path, fstream::in | fstream::out | fstream::trunc);

    if (!configFile)
    {
        configFile.open(config.path + "config.cfg", fstream::in | fstream::out | fstream::trunc);
        configFile << PRINTER(g_Vars.m_Aimbot);
        configFile.close();
    }

}

