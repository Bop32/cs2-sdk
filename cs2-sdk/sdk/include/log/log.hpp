#pragma once
#include <hacks/aimbot/aimbot.hpp>
#include <bindings/playercontroller.hpp>
#include <globals/globals.hpp>
#include <renderer/renderer.hpp>

class NotifyText
{
public:
    float time;
    float duration;
    std::string text;
public:
    NotifyText(std::string& text, float time, float duration) : time(time), duration(duration), text(text) {}
};

class Notify
{
public:
    void Add(std::string& text, float time, float duration = 3.0f)
    {
        notifyLog.push_back(std::make_shared<NotifyText>(text, time, duration));
    }

    void NotifyLogic()
    {
        auto drawList = CRenderer::GetBackgroundDrawList();

        for (int i = 0; i < notifyLog.size(); i++)
        {
            auto& element = notifyLog[i];

            float newTime = globals::localPlayerController->m_nTickBase() * globals::GlobalVars->interval_per_tick;
            newTime -= element->time;
            if (newTime >= element->duration)
            {
                notifyLog.erase(notifyLog.begin() + i);
                continue;
            }
            drawList->AddText({10, (i * 12.0f)}, IM_COL32(255, 255, 255, 255), element->text.c_str());
        }
    }

private:
    std::vector<std::shared_ptr< NotifyText >> notifyLog;

};

extern Notify notify;

class Log
{
public:
    static const char* HitGroupToString(int hitgroup)
    {
        switch (hitgroup)
        {
            case HITGROUP::HITGROUP_GENERIC:
                return "generic";
            case HITGROUP::HITGROUP_HEAD:
                return "head";
            case HITGROUP::HITGROUP_CHEST:
                return "chest";
            case HITGROUP::HITGROUP_STOMACH:
                return "stomach";
            case HITGROUP::HITGROUP_LEFTARM:
                return "arm";
            case HITGROUP::HITGROUP_RIGHTARM:
                return "arm";
            case HITGROUP::HITGROUP_LEFTLEG:
                return "leg";
            case HITGROUP::HITGROUP_RIGHTLEG:
                return "leg";
            case HITGROUP::HITGROUP_NECK:
                return "neck";
            default:
                return "gear";
        }
    }

};

