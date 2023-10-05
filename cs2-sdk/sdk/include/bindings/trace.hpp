#pragma once


#include <math/types/vector.hpp>

#include <bindings/playerpawn.hpp>

#include <memory/memory.hpp>

#include <cstdint>


namespace trace
{

    struct C_Ray
    {
        Vector Start;
        Vector End;
        Vector Mins;
        Vector Maxs;
        char pad1[0x4];
        std::uint8_t UnkType;
    };

    struct C_SurfaceData
    {
        char pad1[0x8];
        float PenetrationModifier;
        float DamageModifier;
        char pad2[0x4];
        int Material;
    };

    struct C_TraceHitboxData
    {
        char pad1[0x58];
        int Hitgroup;
        char pad2[0x4];
        int HitboxId;
    };

    class C_GameTrace
    {
    public:
        C_SurfaceData* GetSurfaceData();

        int GetHitboxId()
        {
            if (HitboxData) return HitboxData->HitboxId;
            return 0;
        }

        int GetHitgroup()
        {
            if (HitboxData) return HitboxData->Hitgroup;
            return 0;
        }

        void* Surface;
        C_CSPlayerPawnBase* HitEntity;
        C_TraceHitboxData* HitboxData;
        char pad1[0x10];
        int32_t Contents;
        char pad2[0x58];
        Vector EndPos;
        char pad3[0x1C];
        float Fraction;
        char pad4[0x10];
    };

    class C_TraceFilter
    {
    public:
        uint64_t TraceMask;
        uint64_t V1[2];
        uint32_t SkipHandles[4];
        uint16_t Collisions[2];
        uint16_t V2;
        uint8_t V3;
        uint8_t V4;
        uint8_t V5;

        virtual ~C_TraceFilter() {}
        virtual bool function() { return true; }

        C_TraceFilter(std::uint32_t Mask, C_CSPlayerPawnBase* Skip1,
            C_CSPlayerPawnBase* Skip2, int Layer);
    };

    class TraceManager
    {
    public:
        static TraceManager* Get()
        {
            auto value = *reinterpret_cast< TraceManager** >(signatures::TraceManager.GetPtrAs<TraceManager*>());
            return value;
        }
    };

};  // namespace os2::sdk
