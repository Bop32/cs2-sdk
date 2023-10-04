#pragma once
#include "cusercmd.hpp"

class CCSGOInput {
   public:                      
    static CCSGOInput* Get();

    void* table;
    char pad1[0x248];
    CUserCmd m_commands[150];
    char pad2[0x1];
    bool m_in_third_person;
    char pad3[0x22];
    int32_t m_sequence_number;

    auto GetUserCmd()
    {
        return &m_commands[m_sequence_number % 150];
    }

    /*
    auto SetViewAngles(CmdQAngle* angle)
    {
        //auto angles = signatures::SetViewAngles.GetPtrAs<CmdQAngle*>();
        //angles = angle;
        return;
    }
    */

};
