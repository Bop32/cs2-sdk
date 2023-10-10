#pragma once

#include <virtual/virtual.hpp>


namespace material
{
    class CMaterial2
    {
    public:
        const char* GetName()
        {
            return CALL_VIRTUAL(const char*, 0, this);
        }

        const char* GetShareName()
        {
            return CALL_VIRTUAL(const char*, 1, this);
        }
    };

    class CMaterialSystem2
    {
    public:
        CMaterial2*** FindMaterial(CMaterial2*** material, const char* material_name)
        {
            return CALL_VIRTUAL(CMaterial2***, 14, this, material, material_name);
        }

        CMaterial2** CreateMaterial(CMaterial2*** empty_material, const char* new_material_name, void* material_data)
        {
            return CALL_VIRTUAL(CMaterial2**, 29, this, empty_material, new_material_name, material_data, 0, 0, 0, 0, 0, 1);
        }

        void SetCreateDataByMaterial(void* data, CMaterial2*** material)
        {
            return CALL_VIRTUAL(void, 37, this, material, data);
        }
    };
};
