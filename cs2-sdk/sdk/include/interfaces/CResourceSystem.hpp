#pragma once
#include <cstdint>
#include <material/material.hpp>
struct ResourceArray_t
{
    uint64_t m_nCount;			// 0x0
    material::CMaterial2*** m_aResources;	// 0x8
    uint64_t pad_0010[3];		// 0x10
};
class CResourceSystem
{
public:
    static CResourceSystem* Get();
    void EnumerateResources(uint64_t iTypeHash, ResourceArray_t* pResult, uint8_t Flag);
};
