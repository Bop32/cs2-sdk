#pragma once
#include <cstdint>
#include <math/types/vector.hpp>
#include <cstronghandle/CStrongHandle.hpp>
#include <bindings/gamescenenode.hpp>
#include <schemamgr/schema_manager.hpp>
#include <signatures/signatures.hpp>

using CUtlSymLargeId = std::int32_t*;

class CUtlSymbolLarge
{
    inline static const CUtlSymLargeId UTL_INVALID_SYMBOL_LARGE =
        reinterpret_cast< CUtlSymLargeId >(-1);

public:
    CUtlSymbolLarge() { id_ = UTL_INVALID_SYMBOL_LARGE; }

    explicit CUtlSymbolLarge(CUtlSymLargeId id) { id_ = id; }

    CUtlSymbolLarge(CUtlSymbolLarge const& sym) { id_ = sym.id_; }

    CUtlSymbolLarge& operator=(CUtlSymbolLarge const& src)
    {
        id_ = src.id_;

        return *this;
    }

    bool operator==(CUtlSymbolLarge const& src) const { return id_ == src.id_; }

    bool operator==(CUtlSymLargeId const& src) const { return id_ == src; }

    bool operator!=(CUtlSymbolLarge const& src) const { return id_ != src.id_; }

    bool operator!=(CUtlSymLargeId const& src) const { return id_ != src; }

    explicit operator CUtlSymLargeId() const { return id_; }

    [[nodiscard]] const char* get_string() const
    {
        if (id_ == UTL_INVALID_SYMBOL_LARGE) return "";

        return string_;
    }

    [[nodiscard]] bool is_valid() const
    {
        return id_ != UTL_INVALID_SYMBOL_LARGE;
    }

private:
    union
    {
        CUtlSymLargeId id_;
        const char* string_;
    };
};

class CModel
{
public:
    int32_t GetBoneFlag(int32_t index);
    int32_t GetBoneParent(int32_t index);
    uint8_t padding_0[0x170];
    int32_t BoneCount;
};
struct alignas(16) CBoneData
{
    Vector position;
    float scale;

    float rotation[0x4];
};

class CModelState
{
public:
    uint8_t padding_0[0x80];
    CBoneData* bones;
    uint8_t padding_1[0x18];
    CStrongHandle<CModel> modelHandle;
    CUtlSymbolLarge modelName;
};

class CSkeletonInstance : public CGameSceneNode
{
public:
    SCHEMA(CModelState, m_modelState, "CSkeletonInstance", "m_modelState");
    SCHEMA(uint8_t, m_nHitboxSet, "CSkeletonInstance", "m_nHitboxSet");

    inline void calcWorldSpaceBones(int32_t mask) {
        using function_t = void(__fastcall*)(CSkeletonInstance*, int32_t);
        static function_t fn = reinterpret_cast<function_t>(signatures::CalcWorldSpaceBones.GetPtrAs<void*>());

        fn(this, mask);
    }
};
