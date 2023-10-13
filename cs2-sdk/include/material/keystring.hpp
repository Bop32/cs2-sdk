#pragma once
#include <cstddef>

class KeyString
{
public:
    uintptr_t mKey;
    const char* mString;

    KeyString(const char* string, bool finding_key = false)
    {
        mString = string;
        mKey = finding_key ? FindKey(string) : -1;
    }

    KeyString(const char* string, int key)
    {
        mString = string;
        mKey = key;
    }

    uintptr_t FindKey(const char* name)
    {
        using type = uintptr_t(__fastcall*)(const char*, int, int);
        type fn = ( type )signatures::FindKeyHook.GetPtrAs<void*>(); // particles.dll E8 ? ? ? ? 80 7F 1A 00 + 1
        return fn(name, 0x12, 0x31415926);
    }

};
static void SetColor(void* data, float* color)
{
    *( byte* )(( uintptr_t )data + 0x40) = color[0] * 255;
    *( byte* )(( uintptr_t )data + 0x41) = color[1] * 255;
    *( byte* )(( uintptr_t )data + 0x42) = color[2] * 255;
    *( byte* )(( uintptr_t )data + 0x43) = color[3] * 255;
}

static void SetMaterialShaderType(void* data, const char* shader_name)
{
    using type = void(__fastcall*)(void*, KeyString, const char*, int);

    type fn = ( type )signatures::SetMaterialShaderHook.GetPtrAs<void*>(); // particles.dll  E8 ? ? ? ? 48 8D B7 ? ? ? ? + 1

    KeyString shader(( const char* )"shader", 0x162C1777);
    fn(data, shader, shader_name, 0x18);
}

static void SetMaterialFunctions(void* data, const char* functions_name, int value)
{
    using type = void(__fastcall*)(void*, KeyString, int, int);
    type set_other = ( type )signatures::SetMaterialFunctionHook.GetPtrAs<void*>(); //particles.dll  E8 ? ? ? ? 49 C1 E5 21 + 1

    KeyString functions(functions_name, true);

    set_other(data, functions, value, 0x12);
}

static CMaterial2* CreateMaterialVisible()
{
    CMaterialSystem2* material_system = CMaterialSystem2::Get();

    byte* buffer[0x200] = {};
    void* data = buffer + 0x50;

    CMaterial2** material_prototype;

    material_system->FindMaterial(&material_prototype, "materials/dev/primary_white.vmat");

    material_system->SetCreateDataByMaterial(data, &material_prototype);

    SetMaterialShaderType(data, "csgo_unlitgeneric.vfx");

    SetMaterialFunctions(data, "F_BLEND_MODE", 1);

    SetMaterialFunctions(data, "F_TRANSLUCENT", true);

    CMaterial2** custom_material;

    material_system->CreateMaterial(&custom_material, "visible_material", data);

    return *custom_material;
}

static CMaterial2* CreateMaterialInvisible()
{
    CMaterialSystem2* material_system = CMaterialSystem2::Get();

    byte* buffer[0x200] = {};
    void* data = buffer + 0x50;

    CMaterial2** material_prototype;

    material_system->FindMaterial(&material_prototype, "materials/dev/primary_white.vmat");

    material_system->SetCreateDataByMaterial(data, &material_prototype);

    SetMaterialShaderType(data, "csgo_unlitgeneric.vfx");

    SetMaterialFunctions(data, "F_DISABLE_Z_BUFFERING", true);

    SetMaterialFunctions(data, "F_BLEND_MODE", 1);

    SetMaterialFunctions(data, "F_TRANSLUCENT", true);

    CMaterial2** custom_material;

    material_system->CreateMaterial(&custom_material, "materials/dev/glowproperty.vmat", data);

    return *custom_material;
}

