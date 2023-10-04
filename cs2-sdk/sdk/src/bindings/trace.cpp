#include <bindings/trace.hpp>
#include <interfaces/gameentitysystem.hpp>

c_trace_filter::c_trace_filter(std::uint64_t trace_mask, CCSPlayerController* local_player, std::uint8_t layer)
{
    this->trace_mask = trace_mask;

    this->null_it1 = 0;
    this->null_it2 = 0;
    //this->skip_handle1 = CGameEntitySystem::GetHandleFromEntity(local_player);
    this->skip_handle2 = 0;
    this->skip_handle3 = 0;
    this->skip_handle4 = 0;

    //this->collision1 = local_player->m_pCollision()->get_collision_mask();
    this->collision2 = 0;

    this->N0000011C = 7;

    this->layer = layer;

    this->N00000104 = 0x49;
    this->null_it3 = 0;
}
