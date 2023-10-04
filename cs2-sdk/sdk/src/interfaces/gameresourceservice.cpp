#include "pch.hpp"

#include <interfaces/gameresourceservice.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

CGameResourceService* CGameResourceService::Get() {
    static const auto inst = CMemory::GetInterface(CConstants::ENGINE_LIB, "GameResourceServiceClientV001");
    return inst.Get<CGameResourceService*>();
}

CGameEntitySystem* CGameResourceService::GetGameEntitySystem() {
    return CPointer(this).GetField<CGameEntitySystem*>(0x58 - LINUX_OFFSET(8));
}
