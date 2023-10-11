#pragma once
#include <material/material.hpp>

using namespace material;

class CMaterialSystem2
{
public:
    static CMaterialSystem2* Get();

    CMaterial2*** FindMaterial(CMaterial2*** material, const char* material_name);

    CMaterial2** CreateMaterial(CMaterial2*** empty_material, const char* new_material_name, void* material_data);

    void SetCreateDataByMaterial(void* data, CMaterial2*** material);
};
