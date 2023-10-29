#pragma once

#include <math/types/transform.hpp>
#include <virtual/virtual.hpp>
#include <schemamgr/schema_manager.hpp>

class CSkeletonInstance;

class CGameSceneNode
{
public:
    SCHEMA(CTransform, m_nodeToWorld, "CGameSceneNode", "m_nodeToWorld");
    SCHEMA(Vector, m_vecAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin");
    SCHEMA(bool, m_bDormant, "CGameSceneNode", "m_bDormant");

    CSkeletonInstance* GetSkeletonInstance()
    {
        auto fn = reinterpret_cast< CSkeletonInstance*(__fastcall*)(void*) >(vt::GetMethod(this, 8).Get<CSkeletonInstance*>());
        return fn(this);
    }
};
