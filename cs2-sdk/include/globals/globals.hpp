#pragma once
#include <bindings/playercontroller.hpp>
#include <interfaces/gameentitysystem.hpp>
#include <hacks/aimbot/aimbot.hpp>
class Globals;

#define TICK_INTERVAL			(GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )


using namespace aimbot;
namespace globals
{
    class CGlobalVarsBase
    {
    public:
        float real_time;
        std::int32_t frame_count;
        std::uint8_t padding_0[0x8];
        std::int32_t max_clients;
        float interval_per_tick;
        std::uint8_t padding_1[0x14];
        float current_time;
        float current_time_2;
        std::uint8_t padding_2[0xC];
        std::int32_t tick_count;
        float interval_per_tick_2;
        std::uint8_t padding_3[0x138];
        std::uint64_t current_map;
        std::uint64_t current_map_name;
    };
    inline CCSPlayerController* localPlayerController = nullptr;
    inline C_CSPlayerPawnBase* localPlayerPawn = nullptr;
    inline CGlobalVarsBase* GlobalVars = nullptr;
    inline AimbotData aimbotData;
    inline CUserCmd* cmd = nullptr;
}
