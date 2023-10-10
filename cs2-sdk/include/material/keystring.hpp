#pragma once
#include <cstddef>

class KeyString
{
public:
    uintptr_t mKey;
    const char* mString;

    KeyString(const char* string, bool finding_key = false)
    {
        mString = string;
        mKey = finding_key ? FindKey(string) : -1;
    }

    KeyString(const char* string, int key)
    {
        mString = string;
        mKey = key;
    }

    uintptr_t FindKey(const char* name)
    {
        using type = uintptr_t(__fastcall*)(const char*, int, int);
        //type fn = ( type )memory::find_var_material; // particles.dll E8 ? ? ? ? 80 7F 1A 00 + 1
        //return fn(name, 0x12, 0x31415926);
    }

};
static void SetColor(void* data, float* color)
{
    *( std::byte* )(( uintptr_t )data + 0x40) = color[0] * 255;
    *( std::byte* )(( uintptr_t )data + 0x41) = color[1] * 255;
    *( std::byte* )(( uintptr_t )data + 0x42) = color[2] * 255;
    *( std::byte* )(( uintptr_t )data + 0x43) = color[3] * 255;
}

