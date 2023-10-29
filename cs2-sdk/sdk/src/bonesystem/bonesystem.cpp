#include <pch.hpp>
#include <bonesystem/bonesystem.hpp>
#include <signatures/signatures.hpp>

int32_t CModel::GetBoneFlag(int32_t index)
{
    using function_t = int32_t(__fastcall*)(CModel*, int32_t);
    static function_t fn = reinterpret_cast< function_t >(signatures::GetBoneFlags.GetPtrAs<void*>());

    return fn(this, index);
}

int32_t CModel::GetBoneParent(int32_t index)
{
    using function_t = int32_t(__fastcall*)(CModel*, int32_t);
    static function_t fn = reinterpret_cast< function_t >(signatures::GetBoneParent.GetPtrAs<void*>());

    return fn(this, index);
}
