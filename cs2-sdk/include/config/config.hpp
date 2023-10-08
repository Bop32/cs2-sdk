#pragma once
#include <vars/vars.hpp>

using namespace std;

class Config
{
public:
    static Config& Get()
    {
        static Config inst;
        return inst;
    }
    void SaveConfig();
    void CheckDirectory();

private:
    char* userpath = getenv("USERPROFILE");
    string directoryPath = string(userpath) + "\\Documents\\cs2-sdk";
    string path = string(userpath) + "\\Documents\\cs2-sdk\\";
};
