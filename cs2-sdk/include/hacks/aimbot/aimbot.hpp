#pragma once
#include <input/cusercmd.hpp>
#include <bindings/playerpawn.hpp>


enum HITGROUP
{
    HITGROUP_INVALID = -1,
    HITGROUP_GENERIC = 0,
    HITGROUP_HEAD = 1,
    HITGROUP_CHEST = 2,
    HITGROUP_STOMACH = 3,
    HITGROUP_LEFTARM = 4,
    HITGROUP_RIGHTARM = 5,
    HITGROUP_LEFTLEG = 6,
    HITGROUP_RIGHTLEG = 7,
    HITGROUP_NECK = 8,
    HITGROUP_UNUSED = 9,
    HITGROUP_GEAR = 10,
    HITGROUP_SPECIAL = 11,
    HITGROUP_COUNT = 12,
};

enum HITBOXES : DWORD
{
    HEAD = 6,
    NECK_0 = 5,
    SPINE_1 = 4,
    SPINE_2 = 2,
    PELVIS = 0,
    ARM_UPPER_L = 8,
    ARM_LOWER_L = 9,
    HAND_L = 10,
    ARM_UPPER_R = 13,
    ARM_LOWER_R = 14,
    HAND_R = 15,
    LEG_UPPER_L = 22,
    LEG_LOWER_L = 23,
    ANKLE_L = 24,
    LEG_UPPER_R = 25,
    LEG_LOWER_R = 26,
    ANKLE_R = 27,
};

namespace aimbot
{
    struct AimbotData
    {
        AimbotData(C_CSPlayerPawnBase* enemy, Vector& shotPosition, Vector& angle, bool canFire) :
            enemy(enemy),
            shotPosition(shotPosition),
            angle(angle),
            canFire(canFire)
        {
        }

        AimbotData()
        {
            enemy = nullptr;
            shotPosition = {};
            angle = {};
            canFire = false;
        }

        C_CSPlayerPawnBase* enemy = nullptr;
        Vector shotPosition {};
        Vector angle {};
        bool canFire = false;
    };

    void RunAimbot(CUserCmd* cmd);
    bool HitChance(C_CSPlayerPawnBase* enemy, Vector& angle, C_BasePlayerWeapon* weapon, float weaponRange);
}
