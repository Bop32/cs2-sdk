#pragma once

class CCollisionProperty
{
public:
    SCHEMA(Vector, m_vecMins, "CCollisionProperty", "m_vecMins");
    SCHEMA(Vector, m_vecMaxs, "CCollisionProperty", "m_vecMaxs");
    SCHEMA(uint16_t, m_solidFlags, "CCollisionProperty", "m_usSolidFlags");

    auto CollisionMask()
    {
        return *reinterpret_cast< std::uint16_t* >(( uintptr_t )(this) + 0x38);
    }
};
