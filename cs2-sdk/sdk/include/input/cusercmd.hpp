#pragma once
#include "../../../include/math/types/vector.hpp"

class CmdQAngle
{
public:
    char pad1[24];
    Vector angles;
    char pad2[1052];
}; //Size: 0x0440

struct CBaseUserCmd
{
    char pad1[0x40];
    CmdQAngle* view;
};

class CUserCmd
{
public:
    enum : unsigned __int64
    {
        IN_ATTACK = 1 << 0,
        IN_JUMP = 1 << 1,
        IN_DUCK = 1 << 2,
        IN_FORWARD = 1 << 3,
        IN_BACK = 1 << 4,
        IN_USE = 1 << 5,
        IN_TURNLEFT = 1 << 7,
        IN_TURNRIGHT = 1 << 8,
        IN_MOVELEFT = 1 << 9,
        IN_MOVERIGHT = 1 << 10,
        IN_ATTACK2 = 1 << 11,
        IN_RELOAD = 1 << 13,
        IN_SPEED = 1 << 16,
        IN_JOYAUTOSPRINT = 1 << 17,
        IN_FIRST_MOD_SPECIFIC_BIT = ( unsigned __int64 )1 << 32,
        IN_USEORRELOAD = ( unsigned __int64 )1 << 32,
        IN_SCORE = 8589934592,
        IN_ZOOM = 17179869184,
        IN_JUMP_THROW_RELEASE = 34359738368,
    };

    /*
    PAD(0x20);
    SubTicksContainer m_sub_tick_container; //20
    CBaseUserCmd* m_base_cmd; //30
    int m_start_history_index_attack1; //38
    int m_start_history_index_attack2; //0x3C
    int m_start_history_index_attack3; //0x40
    PAD(0xC); //0x44
    uint64_t m_buttons; //0x48
    uint64_t m_buttons_changed;
    uint64_t m_buttons_scroll;
    PAD(0x8);
    */
    char pad1[0x30];
    CBaseUserCmd* base;
    char pad2[0x18];
    unsigned int buttons;
};
