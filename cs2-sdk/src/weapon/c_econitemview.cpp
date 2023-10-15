#include <pch.hpp>
#include <weapon/c_econitemview.hpp>
#include <signatures/signatures.hpp>

using namespace weapon;
CCSWeaponBaseVData* C_EconItemView::GetWeaponInfo()
{
    using function_t = CCSWeaponBaseVData * (__fastcall*)(void*);
    static function_t fn = reinterpret_cast< function_t >(signatures::WeaponInfo.GetPtrAs<CCSWeaponBaseVData*>());

    return fn(this);
}

