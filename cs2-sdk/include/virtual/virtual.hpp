#pragma once

#include <pointer/pointer.hpp>

#define CALL_VIRTUAL(retType, idx, ...) \
    vt::CallVirtual<retType>(idx, __VA_ARGS__)

namespace vt
{
    CPointer GetMethod(CPointer base, uint32_t index);

    template <typename T, typename... Args>
    inline T CallMethod(CPointer base, uint32_t index, Args... args)
    {
        return GetMethod(base, index).Call<T(*)(void*, Args...)>(base.Get<void*>(), std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    inline T CallVirtual(uint32_t uIndex, void* pClass, Args... args)
    {
        auto pFunc = GetVMethod<T(__thiscall*)(void*, Args...)>(uIndex, pClass);
        if (!pFunc)
        {
            return T {};
        }

        return pFunc(pClass, args...);
    }
}  // namespace vt
