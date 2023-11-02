#pragma once
#include <imgui/imgui.h>
#include <math/types/vector.hpp>
#include <virtual/virtual.hpp>

class CBasePB
{
    void* vftable;
    uint32_t has_bits;
    uint64_t cached_size;
};

static_assert(sizeof(CBasePB) == 0x18);


template< typename T >
struct RepeatedPtrField
{
    struct Rep
    {
        int allocated_size;
        T* elements[(std::numeric_limits< int >::max() - 2 * sizeof(int)) /
            sizeof(void*)];
    };

    void* arena;
    int current_size;
    int total_size;
    Rep* rep;
};

class CMsgVector : public CBasePB
{
public:
    ImVec4 position;

};

class CMsgQAngle : public CBasePB
{
public:
    Vector angles;
};


class CSGOInterpolationInfoPB : public CBasePB
{
    float frac;
    int32_t src_tick;
    int32_t dst_tick;
};


struct CSGOInputHistoryEntryPB : public CBasePB
{
    CMsgQAngle* m_angle_message; //0x0018
    CMsgVector* m_shoot_position; //0x0020
    CMsgVector* m_target_head_position; //0x0028 I wonder guys I wonder
    CMsgVector* m_target_abs_origin_message; //0x0030
    CMsgQAngle* m_target_angle_message; //0x0038
    CSGOInterpolationInfoPB* m_interpolation_messages[4]; //0x0040
    int32_t m_frame_tick_count; //0x0060
    float m_frame_tick_fraction; //0x0064
    int32_t m_player_tickbase; //0x0068
    float m_player_tickbase_fraction; //0x006C
    int32_t m_frame_count; //0x0070 I think
    int32_t m_target_index; //0x0074 <--- pay attention to this

    CMsgVector* GetAllocationParameter()
    {
        CMsgVector tmp;
        auto fn = reinterpret_cast< CMsgVector* (__fastcall*)(void*, CMsgVector*) >(vt::GetMethod(this, 5).Get<CSGOInputHistoryEntryPB*>());
        return fn(this, &tmp);
    }
};

static_assert(sizeof(CSGOInputHistoryEntryPB) == 0x78);

struct CSGOInputMessage
{
    int32_t m_frame_tick_count; //0x0000
    float m_frame_tick_fraction; //0x0004
    int32_t m_player_tick_count; //0x0008
    float m_player_tick_fraction; //0x000C
    Vector m_view_angles; //0x0010
    Vector m_shoot_position; //0x001C
    int32_t m_target_index; //0x0028
    Vector m_target_head_position; //0x002C What's this???
    Vector m_target_abs_origin; //0x0038
    Vector m_target_angle; //0x0044
    int32_t m_sv_show_hit_registration; //0x0050
    int32_t m_entry_index_max; //0x0054
    int32_t m_index_idk; //0x0058
}; //Size: 0x005C

static_assert(sizeof(CSGOInputMessage) == 0x005C);

class CCSGOUserCmdPB
{
public:
    int32_t nTickCount;
    char pad1[0x4];
    uint8_t* tickPointer;

    CSGOInputHistoryEntryPB* GetInputHistoryEntry(std::int32_t nTick)
    {
        if (nTick < this->nTickCount)                 
        {
            CSGOInputHistoryEntryPB** arrTickList = reinterpret_cast< CSGOInputHistoryEntryPB** >(reinterpret_cast< std::uintptr_t >(tickPointer) + 0x8);
            return arrTickList[nTick];
        }

        return nullptr;
    }
};
static_assert(sizeof(CCSGOUserCmdPB) == 0x10);

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
    CMsgQAngle* view;
    int m_command_number;
    int m_tick_count;
    float m_forwardmove;
    float m_rightmove;
    float m_upmove;
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
    CCSGOUserCmdPB csgoUserCmd;
    CBaseUserCmd* base;
    int startHistoryIndexAttack1;
    int startHistoryIndexAttack2;
    int startHistoryIndexAttack3;
    char pad2[0x8];
    uint64_t buttons;
    uint64_t buttonsChanged;
    uint64_t buttonsScroll;
    char pad3[0x8];

    void SetSubTickAngles(CUserCmd* cmd, Vector& angles)
    {
        for (int i = 0; i < this->csgoUserCmd.nTickCount; i++)
        {
            auto* tick = this->csgoUserCmd.GetInputHistoryEntry(i);

            if (tick && tick->m_angle_message)
            {
                tick->m_angle_message->angles = angles;
            }
        }
    }
};
static_assert(sizeof(CUserCmd) == 0x70);
