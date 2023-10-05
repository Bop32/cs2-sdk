#pragma once

#include <bindings/baseentity.hpp>

class C_CSPlayerPawnBase : public C_BaseEntity
{
public:
    bool IsPlayerPawn();
    bool IsObserverPawn();


    uint32_t GetOwnerHandle()
    {
        std::uint32_t Result = -1;
        if (this && m_pCollision() && !(m_pCollision()->m_solidFlags() & 4))
        {
            Result = this->m_hOwnerEntity().m_Index;
        }
        return Result;
    }

    uint32_t GetCollisionMask()
    {
        if (this && m_pCollision())
            return m_pCollision()->CollisionMask();  // Collision + 0x38
        return 0;
    }
};
