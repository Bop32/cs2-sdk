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
            if (HitboxData) return HitboxData->m_nGroupId;
            return 0;
        }

        int GetHitGroup()
        {
            if (HitboxData) return HitboxData->m_nGroupId;
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

        void* Surface; //0x0000
        C_CSPlayerPawnBase* HitEntity; //0x0008
        CHitBox* HitboxData; //0x0010
        char pad_0017[16]; //0x0018
        int32_t Contents;  //0x0028
        char pad_0018[36]; //0x002C
        uint32_t surface_flags; //0x0050
        char pad_0054[36]; //0x0054
        Vector startPos; //0x0078
        Vector EndPos; //0x0084
        Vector normal; //0x0090
        Vector another_endpos; //0x009C
        char pad_00A8[4]; //0x00A8
        float Fraction; //0x00AC
        char pad_00B0[6]; //0x00B0
        bool allSolid; //0x00B6
        char pad_00B7[77]; //0x00B7
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
        char pad4[0x6];  // 0xB0
        bool allsolid;   // 0xB6
                                        */
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
