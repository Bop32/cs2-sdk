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
        int frame_count;
        float unknown1;
        float unknown2;
        int max_clients;
        float interval_per_tick;
        int unknown3;
        int unknown4;
        void* m_unkfunc;
        float unknown5;
        float current_time;
        float current_time2;
        char pad1[0xC];
        int tick_count;
        float interval_per_tick2;
        void* current_netchan;
        char pad2[0x130];
        char* current_map;
        char* current_map_name;
    };
    inline CCSPlayerController* local_player = nullptr;
    inline CGlobalVarsBase* GlobalVars = nullptr;
}
