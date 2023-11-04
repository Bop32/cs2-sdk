#pragma once
#include "cusercmd.hpp"
#include <signatures/signatures.hpp>

class CCSGOInput
{
public:
    static CCSGOInput* Get();

    char pad_0000[0x250];       // 0x0000
    CUserCmd commands[150];    // 0x0250
    char pad_43F0[0x1];         // 0x43F0
    bool in_third_person;       // 0x43F1
    char pad_43F2[0x22];        // 0x43F2
    int32_t command_number;     // 0x4414
    uint64_t unkbutton1;        // 0x4418 seems to be the same as buttonstate1?
    uint64_t unkbutton2;        // 0x4420 appears to be always 0
    uint64_t unkbutton3;        // 0x4428 appears to be always 0
    uint64_t held_buttons;      // 0x4430 assigned to buttonstate1
    float forwardmove;          // 0x4438
    float leftmove;             // 0x443C
    float upmove;               // 0x4440
    int32_t mousedx;            // 0x4444
    int32_t mousedy;            // 0x4448
    int32_t subtick_count;      // 0x444C unk name
    uint64_t button;            // 0x4450
    bool pressed;               // 0x4458
    char pad_4455[0x3];         // 0x4459 compiler padding
    float when;                 // 0x445C
    char pad_4460[0xB0];        // 0x4460
    Vector viewangles;         // 0x4510

    CUserCmd* GetUserCmd()
    {
        return &commands[command_number % 150];
    }

    void SetViewAngles(Vector angle)
    {
        using function_t = void(__fastcall*)(CCSGOInput*, std::int32_t, Vector&);
        static function_t fn = reinterpret_cast< function_t >(signatures::SetViewAngles.GetPtrAs<void*>());

        fn(this, 0, angle);
    }

    void GetViewAngles(Vector& angle)
    {
        using function_t = void* (__fastcall*)(CCSGOInput*, std::int32_t);
        static function_t fn = reinterpret_cast< function_t >(signatures::GetViewAngles.GetPtrAs<void*>());

        angle = *reinterpret_cast< Vector* >(fn(this, 0));
    }

};
