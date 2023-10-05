#include "pch.hpp"

#include <interfaces/gameentitysystem.hpp>
#include <interfaces/gameresourceservice.hpp>

#include <signatures/signatures.hpp>

CGameEntitySystem* CGameEntitySystem::Get() { return CGameResourceService::Get()->GetGameEntitySystem(); }

CCSPlayerController* CGameEntitySystem::GetLocalPlayerController()
{
    return signatures::GetLocalPlayer.GetPtr().Call< CCSPlayerController * (*)(int)>(-1);
}

CCSPlayerController* CGameEntitySystem::GetPLayerControllerByIndexInternal(int index)
{

    /*
    using function_t = CCSPlayerController * (__fastcall*)(CGameEntitySystem*, int);
    static function_t fn = reinterpret_cast< function_t >(signatures::GetControllerBasedOnIndex.GetPtrAs<CCSPlayerController*>());

    return fn(this, index);
    */
    return signatures::GetControllerBasedOnIndex.GetPtr().Call<CCSPlayerController * (*)(void*, int)>(this, index);
}


C_BaseEntity* CGameEntitySystem::GetBaseEntityInternal(int index)
{
    return signatures::GetBaseEntity.GetPtr().Call<C_BaseEntity * (*)(void*, int)>(this, index);
}


int CGameEntitySystem::GetHighestEntityIndexInternal()
{
    int highestIdx = -1;

#ifdef _WIN32
    signatures::GetHighestEntityIndex.GetPtr().Call<void (*)(void*, int*)>(this, &highestIdx);
#elif __linux__
    highestIdx = signatures::GetHighestEntityIndex.GetPtr().Call<int (*)(void*)>(this);
#endif

    return highestIdx;
}
