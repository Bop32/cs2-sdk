#include <pch.hpp>
#include <interfaces/CLocalize.hpp>
#include <constants/constants.hpp>
#include <memory/memory.hpp>

CLocalize* CLocalize::Get()
{
    static const auto inst = CMemory::GetInterface(CConstants::LOCALIZE_DLL, "Localize_001");
    return inst.Get<CLocalize*>();
}
