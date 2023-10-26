#pragma once


#include <math/types/vector.hpp>

#include <bindings/playerpawn.hpp>

#include <memory/memory.hpp>

#include <cstdint>


namespace trace
{

    struct C_Ray
    {
        Vector Start;  // 0x0
        Vector End;    // 0xC
        Vector Mins;   // 0x18
        Vector Maxs;   // 0x24
        char pad1[0x4]; //0x30
        std::uint8_t UnkType; //0x34
        //size = 0x35
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
            if (pHitBox) return pHitBox->nGroupId;
            return 0;
        }

        int GetHitGroup()
        {
            if (pHitBox) return pHitBox->nGroupId;
            return 0;
        }

        /*
        void* Surface; //0x0
        C_CSPlayerPawnBase* HitEntity; //0x08
        CHitBox* HitboxData;  //0x10
        char pad1[0x10];    //0x18
        int32_t Contents;  //0x28
        char pad2[0x58];   //0x2C
        Vector EndPos;     //0x84
        char pad3[0x1C];  // 0x90
        float Fraction;   // 0xAC
        char pad4[0x10];
        */

        void* pSurfaceProperties; // 0x00
        C_CSPlayerPawnBase* pHitEntity; // 0x08
        CHitBox* pHitBox; // 0x10
        std::byte pad0[0x38]; // 0x18
        std::uint32_t nSurfaceFlags; // 0x50
        std::byte pad1[0x24]; // 0x54
        Vector vecStart; // 0x78 // initial position
        Vector vecEnd; // 0x84 // final position
        Vector vecNormal; // 0x90 // surface normal at impact
        std::byte pad2[0x10]; // 0x9C
        float flFraction; // 0xAC // time completed, 1.0 = didn't hit anything
        std::byte pad3[0x6]; // 0xB0
        std::uint8_t nShapeType; // 0xB6
        bool bStartSolid; // 0xB7 // if true, the initial point was in a solid area
        std::byte pad4[0x9];
    };

    class C_TraceFilter
    {
    public:
        uint64_t TraceMask;       //0x0
        uint64_t V1[2];           //0x8
        uint32_t SkipHandles[4];  //0x18
        uint16_t Collisions[2];   //0x28
        uint16_t V2;              //0x2C
        uint8_t V3;               //0x2E
        uint8_t V4;               //0x2F
        uint8_t V5;               //0x30

        virtual ~C_TraceFilter() {}
        virtual bool function() { return true; }

        C_TraceFilter(std::uint32_t Mask, C_CSPlayerPawnBase* Skip1,
            C_CSPlayerPawnBase* Skip2, int Layer);

        C_TraceFilter()
        {

        }

        void CreateFilter(std::uint32_t Mask, C_CSPlayerPawnBase* Skip1,
            C_CSPlayerPawnBase* Skip2, int Layer)
        {

        }
    };

    class TraceManager
    {
    public:
        static TraceManager* Get()
        {
            auto value = reinterpret_cast< TraceManager* >(signatures::TraceManager.GetPtrAs<TraceManager*>());
            return value;
        }
    };

};  // namespace os2::sdk
