#pragma once
#include "cusercmd.hpp"
#include <signatures/signatures.hpp>

class CCSGOInput
{
public:
    static CCSGOInput* Get();
private:
	void* vftable;
	uint8_t pad1[0x248];
public:
	CUserCmd m_commands[150];
private:
	uint8_t pad2[0x23];
public:
	int32_t iSequenceNumber;
public:
	CUserCmd* GetUserCmd() { return &m_commands[iSequenceNumber % 150]; }

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
