#include <pch.hpp>
#include <material/material.hpp>
#include <interfaces/CMaterialSystem2.hpp>
#include <memory/memory.hpp>
#include <constants/constants.hpp>

CMaterialSystem2* CMaterialSystem2::Get()
{
    static const auto inst = CMemory::GetInterface(CConstants::MATERIALSYSTEM2, "VMaterialSystem2_001");
    return inst.Get<CMaterialSystem2*>();
}

CMaterial2*** CMaterialSystem2::FindMaterial(CMaterial2*** material, const char* material_name)
{
    return vt::CallMethod<CMaterial2***>(this, 14, material, material_name);
    //return CALL_VIRTUAL(CMaterial2***, 14, this, material, material_name);
}

CMaterial2** CMaterialSystem2::CreateMaterial(CMaterial2*** empty_material, const char* new_material_name, void* material_data)
{
    return CALL_VIRTUAL(CMaterial2**, 29, this, empty_material, new_material_name, material_data, 0, 0, 0, 0, 0, 1);
}

void CMaterialSystem2::SetCreateDataByMaterial(void* data, CMaterial2*** material)
{
    return CALL_VIRTUAL(void, 37, this, material, data);
}

