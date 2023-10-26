#include <pch.hpp>
#include <interfaces/CResourceSystem.hpp>
#include <constants/constants.hpp>
#include <memory/memory.hpp>

CResourceSystem* CResourceSystem::Get()
{
    static const auto inst = CMemory::GetInterface(CConstants::RESOURCE_SYSTEM_DLL, "ResourceSystem013");
    return inst.Get<CResourceSystem*>();
}

void CResourceSystem::EnumerateResources(uint64_t iTypeHash, ResourceArray_t* pResult, uint8_t Flag)
{
    auto fn = reinterpret_cast< void(__fastcall*)(void*, uint64_t, ResourceArray_t*, uint8_t) >(vt::GetMethod(this, 38).Get<void*>());

    fn(this, iTypeHash, pResult, Flag);
}
