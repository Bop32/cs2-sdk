#pragma once

#include <bindings/baseentity.hpp>
#include <hacks/Glow/CGlowProperty.hpp>

class C_CSPlayerPawnBase;

class CBasePlayerController : public C_BaseEntity {
   public:
       SCHEMA(CHandle<C_CSPlayerPawnBase>, m_hPawn, "CBasePlayerController", "m_hPawn");
       SCHEMA(uint32_t, m_iDesiredFOV, "CBasePlayerController", "m_iDesiredFOV");
};

class CCSPlayerController : public CBasePlayerController 
{
   public:
    SCHEMA(bool, m_bPawnIsAlive, "CCSPlayerController", "m_bPawnIsAlive");
    SCHEMA(const char*, m_sSanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName");

    
};
