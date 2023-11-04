#pragma once

#include <bindings/baseentity.hpp>
#include <weapon/c_baseweapon.hpp>

class CPlayer_WeaponServices
{
public:
    SCHEMA(CHandle<C_BasePlayerWeapon>, m_hActiveWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon");
};

class C_CSPlayerPawnBase : public C_BaseEntity
{
public:
    bool IsPlayerPawn();
    bool IsObserverPawn();

    SCHEMA(CPlayer_WeaponServices*, m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
    SCHEMA(bool, m_bIsScoped, "C_CSPlayerPawnBase", "m_bIsScoped");


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


