#pragma once
#include <bindings/playercontroller.hpp>
#include <interfaces/gameentitysystem.hpp>
class Globals;

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
    inline CCSPlayerController* local_player = nullptr;
    inline CGlobalVarsBase* GlobalVars = nullptr;
}
