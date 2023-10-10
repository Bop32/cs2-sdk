#pragma once
#include <pch.hpp>
#include <math/types/vector.hpp>

namespace skeleton
{
    struct CBoneData
    {
        Vector Location;
        float Scale;
        Vector Rotation;
    };

    class CModelState
    {
    public:
        std::uint8_t padding_0[0x80];
        CBoneData* bones;
        std::uint8_t padding_1[0x18];
        //CStrongHandle<CModel> modelHandle; add these two ;-)
        //CUtlSymbolLarge modelName;
    };
};
