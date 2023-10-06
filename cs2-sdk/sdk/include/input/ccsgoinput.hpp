#pragma once
#include "cusercmd.hpp"
#include "../../../include/signatures/signatures.hpp"

class CCSGOInput
{
public:
    static CCSGOInput* Get();

    void* table;
    char pad1[0x248];
    CUserCmd m_commands[150];
    char pad2[0x1];
    bool m_in_third_person;
    char pad3[0x22];
    int32_t m_sequence_number;
    uint64_t button_state1;
    uint64_t button_state2;
    uint64_t button_state3;
    uint64_t button_state4;

    auto GetUserCmd()
    {
        return &m_commands[m_sequence_number % 150];
    }

    void SetViewAngles(Vector angle)
    {
        using function_t = std::int64_t(__fastcall*)(CCSGOInput*, std::int32_t, Vector&);
        static function_t fn = reinterpret_cast< function_t >(signatures::SetViewAngles.GetPtrAs<void*>());

        fn(this, 0, angle);
    }

};
