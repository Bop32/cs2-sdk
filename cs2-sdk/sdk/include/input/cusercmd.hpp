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
    int m_command_number;
    int m_tick_count;
    float m_forwardmove;
    float m_rightmove;
    float m_upmove;
};


class CSubTickCmd
{
public:
    char pad1[0x18];
    CmdQAngle* view;
};

class CSubTickContainer
{
public:
    int32_t tickCount;
    char pad1[0x4];
    uint8_t* tickPointer;

    CSubTickCmd* GetTick(std::int32_t index)
    {
        if (index < this->tickCount)
        {
            CSubTickCmd** tick_list = reinterpret_cast< CSubTickCmd** >(this->tickPointer + 0x8);
            return tick_list[index];
        }

        return nullptr;
    }
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


    char pad1[0x20];
    CSubTickContainer subTickContainer;
    CBaseUserCmd* base;
    int startHistoryIndexAttack1;
    int startHistoryIndexAttack2;
    int startHistoryIndexAttack3;
    char pad2[0x8];
    uint64_t buttons;
    uint64_t buttonsChanged;
    uint64_t buttonsScroll;
    char pad3[0x8];

    CSubTickContainer GetSubTickContainer()
    {
        return *reinterpret_cast< CSubTickContainer* >(reinterpret_cast< std::uintptr_t >(this) + 0x20);
    }

    void SetSubTickAngles(CUserCmd* cmd, Vector& angles)
    {
        CSubTickContainer container = cmd->GetSubTickContainer();
        for (int i = 0; i < container.tickCount; i++)
        {
            CSubTickCmd* tick = container.GetTick(i);

            if (tick && tick->view)
            {
                tick->view->angles = angles;
            }
        }
    }
};
static_assert(sizeof(CUserCmd) == 0x70);
