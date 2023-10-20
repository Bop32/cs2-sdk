#include "pch.hpp"

#include <signatures/signatures.hpp>
#include <constants/constants.hpp>

#define STB_OMIT_TESTS
#include <stb/stb.hh>

#define SDK_SIG(sig) stb::simple_conversion::build<stb::fixed_string{sig}>::value

namespace signatures
{
    CSigScan GetBaseEntity("CGameEntitySystem::GetBaseEntity", CConstants::CLIENT_LIB,
        {
#ifdef _WIN32
                               {SDK_SIG("8B D3 E8 ? ? ? ? 48 8B F8 48 85 C0 74 76"), [](CPointer& ptr) { ptr.Absolute(3, 0); }},
                               {SDK_SIG("81 FA ? ? ? ? 77 36")},
#elif __linux__
                               {SDK_SIG("44 89 E6 E8 ? ? ? ? 48 89 C3 48 85 C0 0F 85 ? ? ? ?"), [](CPointer& ptr) { ptr.Absolute(4, 0); }},
#endif
        });

    CSigScan GetHighestEntityIndex("CGameEntitySystem::GetHighestEntityIndex", CConstants::CLIENT_LIB,
        {
#ifdef _WIN32
                                       {SDK_SIG("33 DB E8 ? ? ? ? 8B 08"), [](CPointer& ptr) { ptr.Absolute(3, 0); }},
                                       {SDK_SIG("E8 ? ? ? ? 33 DB 39 5C 24 40"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
#elif __linux__
                                       {SDK_SIG("E8 ? ? ? ? 41 39 C4 7F 50"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
#endif
        });


    CSigScan GetIsDemoOrHLTV("IsDemoOrHLTV", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("48 83 EC 28 48 8B 0D ?? ?? ?? ?? 48 8B 01 FF 90 ?? ?? ?? ?? 84 C0 75 0D")},
        });

    CSigScan GetCSGOInput("CCSGOInput", CConstants::CLIENT_LIB,
        {
#ifdef _WIN32
                              {SDK_SIG("BA ? ? ? ? 48 03 1D ? ? ? ?"), [](CPointer& ptr) { ptr.Absolute(8, 0).Dereference(1); }},
#elif __linux__
                              {SDK_SIG("4C 8D 35 ? ? ? ? 48 8D 55 D4"), [](CPointer& ptr) { ptr.Absolute(3, 0).Dereference(1); }},
#endif
        });

    CSigScan GetMatricesForView("CRenderGameSystem::GetMatricesForView", CConstants::CLIENT_LIB,
        {
#ifdef _WIN32
                                    {SDK_SIG("40 53 48 81 EC ? ? ? ? 49 8B C1")},
                                    {SDK_SIG("48 89 44 24 ? E8 ? ? ? ? 48 8B 9C 24 ? ? ? ? B0 01"),
                                     [](CPointer& ptr) { ptr.Absolute(6, 0); }},
#elif __linux__
                                    {SDK_SIG("55 48 89 D7 4C 89 C2")},
#endif
        });

    CSigScan GetHitboxSet("C_BaseEntity::GetHitboxSet", CConstants::CLIENT_LIB,
        {
#ifdef _WIN32
                              {SDK_SIG("E8 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 44 8D 48 07"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
                              {SDK_SIG("41 8B D6 E8 ? ? ? ? 4C 8B F8"), [](CPointer& ptr) { ptr.Absolute(4, 0); }},
#elif __linux__
                              {SDK_SIG("E8 ? ? ? ? 48 89 85 ? ? ? ? 48 85 C0 74 0F"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
#endif
        });

    CSigScan HitboxToWorldTransforms("C_BaseEntity::HitboxToWorldTransforms", CConstants::CLIENT_LIB,
        {
#ifdef _WIN32
                                         {SDK_SIG("E8 ? ? ? ? 45 33 F6 4C 63 E0"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
#elif __linux__
                                         {SDK_SIG("E8 ? ? ? ? 41 C7 45 ? ? ? ? ? 4C 89 F7"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
#endif
        });


    CSigScan GetLocalPlayer("GetLocalPlayer", CConstants::CLIENT_LIB,
        {
#ifdef _WIN32
                                         {SDK_SIG("E8 ? ? ? ? 41 83 BF ? ? ? ? ?"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
#elif __linux__
                                         {SDK_SIG("E8 ? ? ? ? 41 83 BF ? ? ? ? ?"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
#endif
        });


    //semi works, need to add offset to it.
    CSigScan GetForceJump("GetForceJump", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("48 8B 05 ? ? ? ? 48 8D 1D ? ? ? ? 48 89 45"), [](CPointer& ptr) { ptr.Absolute(3, 0).Offset(48); }},
        });

    CSigScan SetViewAngles("SetViewAngles", CConstants::CLIENT_LIB,
        {
                {SDK_SIG("F2 41 0F 10 00 4C 63 CA")},
        });

    CSigScan GetViewAngles("GetViewAngles", CConstants::CLIENT_LIB,
        {
                {SDK_SIG("4C 63 C2 4B 8D 04 40 4C 8D 0C C1")},
        });

    CSigScan GetBonePosition("GetBonePosition", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1")},
        });

    CSigScan GetEntityHandle("GetEntityHandle", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("48 85 C9 74 ? 48 8B 41 ? 48 85 C0 74 ? 44 8B 40 ? BA ? ? ? ? 8B 48 ? 41 8B C0 83 E1")},
        });

    CSigScan GetSurfaceData("GetSurfaceData", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("E8 ? ? ? ? 48 85 C0 74 ? 44 38 60"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
        });

    CSigScan TraceShape("TraceShape (Vis Checker)", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("E8 ? ? ? ? 80 7D ? ? 75 ? F3 0F 10 45"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
        });

    CSigScan TraceManager("Trace Manager", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("4C 8B 3D ? ? ? ? 24 C9 0C 49 66 0F 7F 45 ?"), [](CPointer& ptr) { ptr.Absolute(3, 0).Dereference(1); }},
        });

    CSigScan GetControllerBasedOnIndex("GetControllerBasedOnIndex", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("E8 ? ? ? ? 41 83 BF ? ? ? ? ?"), [](CPointer& ptr) { ptr.Absolute(1, 0); }},
        });

    CSigScan GlobalVars("Global Vars", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("48 8B 05 ? ? ? ? 48 8B D8 80 78 3D 00 75 1D 80 78 3C 00 75"), [](CPointer& ptr) { ptr.Absolute(3, 0); }},
        });

    CSigScan LevelInit("Level Init", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2")},
        });

    CSigScan OverrideView("Override View", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("48 89 5C 24 ? 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 01")},
        });

    CSigScan BoneParent("BoneParent", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("85 D2 78 17 3B 91 70")},
        });

    CSigScan CL_Bob_Lower_Amt("CLBobAmtLower", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("F3 0F 10 05 ? ? ? ? 45 32 F6 48 8B 47"), [](CPointer& ptr) { ptr.Absolute(4,0); }}
        });

    CSigScan DrawObjectHook("DrawObjectHook", CConstants::SCENESYSTEM_LIB,
        {
            {SDK_SIG("48 8B C4 48 89 50 10 55 41 56")}
        });

    CSigScan FindKeyHook("FindKeyHook", CConstants::PARTICLE_LIB,
        {
           {SDK_SIG("E8 ? ? ? ? 80 7F 1A 00"), [](CPointer& ptr) { ptr.Absolute(1,0); }},
        });

    CSigScan SetMaterialFunctionHook("SetMateiralFunctionHook", CConstants::PARTICLE_LIB,
        {
           {SDK_SIG("E8 ? ? ? ? 49 C1 E5 21"), [](CPointer& ptr) { ptr.Absolute(1,0); }},
        });

    CSigScan SetMaterialShaderHook("SetMaterialShaderHook", CConstants::PARTICLE_LIB,
        {
           {SDK_SIG("E8 ? ? ? ? 48 8D B7 ? ? ? ?"), [](CPointer& ptr) { ptr.Absolute(1,0); }},
        });

    CSigScan WeaponInfo("Weapon Info", CConstants::CLIENT_LIB,
        {
            {SDK_SIG("48 81 EC ? ? ? ? 48 85 C9 75 ? 33 C0 48 81 C4 ? ? ? ? C3 48 89 9C 24")},
        }); 

    CSigScan ClipTraceToPlayers("ClipTraceToPlayers" , CConstants::CLIENT_LIB,
        
        {
            {SDK_SIG("E8 ? ? ? ? 80 BD ? ? ? ? ? 0F 85 ? ? ? ? F3 0F 10 85 ? ? ? ? 8B 85"),  [](CPointer& ptr) { ptr.Absolute(1,0); }}
        });

    CSigScan HandleBulletPenetration("HandleBulletPenetration", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("48 8B C4 44 89 48 20 55 57 41 55")}
        });

    CSigScan TraceToExit("TraceToExit", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("48 8B C4 48 89 58 10 4C 89 48 20")}
        });

    CSigScan FrameStageNotify("FrameStageNotify", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("48 89 5C 24 ? 56 48 83 EC ? 8B 05 ? ? ? ? 8D 5A") }
        });

    CSigScan GetDamageToPoint("GetDamageToPoint", CConstants::CLIENT_LIB,

        {
            {SDK_SIG("4C 8B DC 53 55 41") }
        });
        
};
