#pragma once
#include <math/types/vector.hpp>
#include <imgui/imgui.h>
#include <virtual/virtual.hpp>
#include <signatures/signatures.hpp>

class CDebugOverlayGameSystem
{
public:
    static CDebugOverlayGameSystem* Get()
    {
        static auto DebugOverlay = *signatures::DebugOverlay.GetPtrAs<CDebugOverlayGameSystem**>();
        return DebugOverlay;
    }

    void AddLineOverlay(const Vector& origin, const Vector& dest, const ImU32& color, bool noDethTest, double duration)
    {
        auto fn = reinterpret_cast< void(__thiscall*)(void*, const Vector&, const Vector&, const ImU32&, bool, double) >(vt::GetMethod(this, 12).Get<void*>());
        fn(this, origin, dest, color, noDethTest, duration);
    }
    /*
    void AddBoxEdgesOverlay(const Vector& origin, const Vector& mins, const Vector& maxs, const QAngle& rotation, const Color& color, float duration)
    {
        auto fn = reinterpret_cast< void(__thiscall*)(CDebugOverlayGameSystem*, const Vector&, const Vector&, const Vector&, const Quaternion&, const Color&, bool, double) >(get_vfunction(this, 14));

        fn(this, origin, mins, maxs, Math::AngleQuaternion(rotation), color, true, static_cast< double >(duration));
    }

    void AddBoxFacesOverlay(const Vector& origin, const Vector& mins, const Vector& maxs, const QAngle& rotation, const Color& color, float duration)
    {
        auto fn = reinterpret_cast< void(__thiscall*)(CDebugOverlayGameSystem*, const Vector&, const Vector&, const Vector&, const Quaternion&, const Color&, bool, double) >(get_vfunction(this, 15));

        fn(this, origin, mins, maxs, Math::AngleQuaternion(rotation), color, true, static_cast< double >(duration));
    }

    void AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& maxs, const QAngle& rotation, const Color& color, float duration)
    {
        auto fn = reinterpret_cast< void(__thiscall*)(CDebugOverlayGameSystem*, const Vector&, const Vector&, const Vector&, const QAngle&, int, int, int, int, double) >(get_vfunction(this, 47));

        fn(this, origin, mins, maxs, rotation, color.r, color.g, color.b, color.a, static_cast< double >(duration));
    }

    void AddBoxOverlay2(const Vector& origin, const Vector& mins, const Vector& maxs, const QAngle& rotation, const Color& faceColor, const Color& edgeColor, float duration)
    {
        auto fn = reinterpret_cast< void(__thiscall*)(CDebugOverlayGameSystem*, const Vector&, const Vector&, const Vector&, const QAngle&, const Color&, const Color&, double) >(get_vfunction(this, 48));

        fn(this, origin, mins, maxs, rotation, faceColor, edgeColor, static_cast< double >(duration));
    }

    void AddTextOverlay(const Vector& origin, float duration, const char* text, ...)
    {
        //auto fn = reinterpret_cast< void(__fastcall*)(CDebugOverlayGameSystem*, const Vector&, double, const char*, ...) >(get_vfunction(this, 58));
        CALL_VIRTUAL(void, 58, this, origin, duration, text);
    }

    void AddTextOverlay(const Vector& origin, float duration, int line_offset, const char* text, ...)
    {
        //auto fn = reinterpret_cast< void(__fastcall*)(CDebugOverlayGameSystem*, const Vector&, int, double, const char*, ...) >(get_vfunction(this, 57));

        CALL_VIRTUAL(void, 57, this, origin, line_offset, duration, text);

        //fn(this, origin, line_offset, duration, text);
    }

    */
    void AddTextOverlay(const Vector& origin, float duration, int line_offset, const ImU32& color, const char* text...)
    {
        auto fn = reinterpret_cast< void(__fastcall*)(CDebugOverlayGameSystem*, const Vector&, int, double, char, char, char, char, const char*, ...) >(vt::GetMethod(this, 59).Get<void*>());

        fn(this, origin, duration, line_offset, 255, 255, 255, 255, text);
    }
};
