#include <pch.hpp>
#include <events/CGameEventManager.hpp>

void* CGameEventManager::gameEventManagerClass = nullptr;

void CGameEventManager::Set(void* rcx)
{
    gameEventManagerClass = rcx;
}

CGameEventManager* CGameEventManager::Get()
{
    return ( CGameEventManager*)gameEventManagerClass;
}

/*
void* CGameEventManager::AddListener(void* unk, const char* eventName, unsigned __int8 unk1)
{
    using function_t = void*(__fastcall*)(void*, void*, const char*, unsigned __int8);
    static function_t fn = reinterpret_cast< function_t >(signatures::AddListener.GetPtrAs<void*>());

   return fn(this, unk, eventName, unk1);
}
*/

