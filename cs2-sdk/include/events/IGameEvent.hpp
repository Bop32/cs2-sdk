#pragma once
#include <cstdint>
#include <virtual/virtual.hpp>
#include <bindings/playerpawn.hpp>
#include <bindings/playercontroller.hpp>
#include "CUtlStringToken.hpp"

class IGameEvent
{
public:
    virtual ~IGameEvent() {}

    [[nodiscard]] const char* GetName() const
    {
        //   @ida: client.dll -> U8["48 8B CE FF 50 ? 48 8D 0D" + 0x5]   @XREF: "show_freeze_panel"
        return vt::CallMethod<const char*>(this, 1);
        //return CallVirtual<const char*, 1U>(this);
    }

    [[nodiscard]] bool GetBool(const CUtlStringToken& keyToken, bool bDefault = false) const
    {
        return vt::CallVirtual<bool>(6, this, &keyToken, bDefault);
        //return vt::GetMethod(this, 6u).Get<bool>();
        //return CallVFunc<bool, 6U>(this, &keyToken, bDefault);
    }

    [[nodiscard]] int GetInt(const CUtlStringToken& keyToken, int iDefault = 0) const
    {
        //   @ida CGameEvent::GetInt(const char*, int): client.dll -> ABS["E8 ? ? ? ? 0F B6 4D 77" + 0x1] 
        //   @ida: client.dll -> U8["E8 ? ? ? ? 45 33 C0 48 89 74 24 ? 48 8D 54 24 ? 49 8B CF FF D3 85 C0 8B 44 24 48" - 0x1] / sizeof(std::uintptr_t)   @XREF: "headshot"
        return vt::CallMethod<int>(this, 7, &keyToken, iDefault);
        //return CallVFunc<int, 7U>(this, &keyToken, iDefault);
    }

    [[nodiscard]] std::uint64_t GetUint64(const CUtlStringToken& keyToken, std::uint64_t ullDefault = 0ULL) const
    {
        return vt::CallVirtual<uint64_t>(8, this, &keyToken, ullDefault);
        //return CallVFunc<std::uint64_t, 8U>(this, &keyToken, ullDefault);
    }

    [[nodiscard]] float GetFloat(const CUtlStringToken& keyToken, const float flDefault = 0.0f) const
    {
        //   @ida CGameEvent::GetFloat(const char*, float): client.dll -> ABS["E8 ? ? ? ? 0F 28 D8 89 5C 24 20" + 0x1]
        //   @ida: client.dll -> U8["4C 8B 60 ? 4D 8B C6" + 0x3] / sizeof(std::uintptr_t)   @XREF: "theta"
        return vt::CallVirtual<float>(9, this, &keyToken, flDefault);
        //return CallVFunc<float, 9U>(this, &keyToken, flDefault);
    }

    [[nodiscard]] const char* GetString(const CUtlStringToken& keyToken, const char* szDefault = "") const
    {
        //   @ida: client.dll -> U8["48 8B 78 ? 48 8D 4D 80" + 0x3] / sizeof(std::uintptr_t)   @XREF: "weapon"
        return vt::CallVirtual<const char*>(10, this, &keyToken, szDefault);
        //return CallVFunc<const char*, 10U>(this, &keyToken, szDefault);
    }

    [[nodiscard]] const wchar_t* GetWString(const CUtlStringToken& keyToken, const wchar_t* wszDefault = L"") const
    {
        return vt::CallVirtual<const wchar_t*>(11, this, &keyToken, wszDefault);
        //return CallVFunc<const wchar_t*, 11U>(this, &keyToken, wszDefault);
    }

    [[nodiscard]] const void* GetPtr(const CUtlStringToken& keyToken) const
    {
        return vt::CallVirtual<const void*>(12, this, &keyToken);
        //return CallVFunc<const void*, 12U>(this, &keyToken);
    }

    int GetPlayerIndex(const CUtlStringToken& keyToken)
    {
        //   @ida: client.dll -> U8["4C 8B 70 ? 4C 89 7C 24" + 0x3] / sizeof(std::uintptr_t)   @XREF: "attacker", "assister", "userid"
        int nOutIndex;
        vt::CallVirtual<void>(15, this, &nOutIndex , &keyToken);
        //CallVFunc<void, 15U>(this, &nOutIndex, &keyToken);
        return nOutIndex;
    }

    CBasePlayerController* GetPlayerController(const CUtlStringToken& keyToken)
    {
        //   @ida: client.dll -> ["89 74 24 40 4C 8B A8 ? ? ? ? 40" + 0x7] / sizeof(std::uintptr_t)   @XREF: "killer_controller"
        return vt::CallVirtual<CBasePlayerController*>(16, this, &keyToken);
        //return CallVFunc<CBasePlayerController*, 16U>(this, &keyToken);
    }

    C_CSPlayerPawnBase* GetPlayerPawn(const CUtlStringToken& keyToken)
    {
        //   @XREF: "_pawn"
        //   @ida CGameEvent::GetPlayerPawn(const char*): server.dll -> ABS["E8 ? ? ? ? 48 85 C0 74 0B 48 8B C8 E8 ? ? ? ? 4C 8B F0 41 8B 46 08" + 0x1]   @XREF: "userid"
        return vt::CallVirtual<C_CSPlayerPawnBase*>(17, this, &keyToken);
        //return CallVFunc<C_CSPlayerPawnBase*, 17U>(this, &keyToken);
    }

    void SetBool(const CUtlStringToken& keyToken, const bool bValue)
    {
        //   @ida CGameEvent::SetBool(const char*, bool): server.dll -> ABS["E8 ? ? ? ? 48 8B 0D ? ? ? ? 45 33 C0 48 8B D3 48 8B 01 FF 50 38 48 8B 46 10" + 0x1]   @XREF: "canbuy"
        vt::CallVirtual<void>(20, this, &keyToken, bValue);
        //CallVFunc<void, 20U>(this, &keyToken, bValue);
    }

    void SetInt(const CUtlStringToken& keyToken, const int nValue)
    {
        //   @ida: server.dll -> ["48 8D 4D A7 4C 8B B0 ? ? ? ? FF" + 0x7] / sizeof(std::uintptr_t)
        vt::CallVirtual<void>(21, this, &keyToken, nValue);
        //CallVFunc<void, 21U>(this, &keyToken, nValue);
    }

    void SetUint64(const CUtlStringToken& keyToken, const std::uint64_t ullValue)
    {
        vt::CallVirtual<void>(22, this, &keyToken, ullValue);
        //CallVFunc<void, 22U>(this, &keyToken, ullValue);
    }

    void SetFloat(const CUtlStringToken& keyToken, const float flValue)
    {
        //   @ida: server.dll -> ["48 8B B0 ? ? ? ? 33 D2 44 89 6C 24" + 0x3] / sizeof(std::uintptr_t)   @XREF: "inferno_expire"
        vt::CallVirtual<void>(23, this, &keyToken, flValue);
        //CallVFunc<void, 23U>(this, &keyToken, flValue);
    }

    void SetString(const CUtlStringToken& keyToken, const char* szValue)
    {
        //   @ida: server.dll -> ["48 8D 4D A7 48 8B B8 ? ? ? ? 33" + 0x7] / sizeof(std::uintptr_t)   @XREF: "weapon"
        vt::CallVirtual<void>(24, this, &keyToken, szValue);
       // CallVFunc<void, 24U>(this, &keyToken, szValue);
    }

    void SetWString(const CUtlStringToken& keyToken, const wchar_t* wszValue)
    {
        vt::CallVirtual<void>(25, this, &keyToken, wszValue);
        //CallVFunc<void, 25U>(this, &keyToken, wszValue);
    }

    void SetPtr(const CUtlStringToken& keyToken, const void* pValue)
    {
        vt::CallVirtual<void>(26, this, &keyToken, pValue);
        //CallVFunc<void, 26U>(this, &keyToken, pValue);
    }

    void SetPlayerIndex(const CUtlStringToken& keyToken, const int nIndex)
    {
        //   @ida: server.dll -> ["48 8B B8 ? ? ? ? E8 ? ? ? ? 48 8D 05 ? ? ? ? 44 8B C3 48 8D 55 97 48 89 45 9F 48 8B CE FF D7 48 8D 15" + 0x3] / sizeof(std::uintptr_t)   @XREF: "attacker_pawn"
        vt::CallVirtual<void>(27, this, &keyToken, nIndex);
        //CallVFunc<void, 27U>(this, &keyToken, nIndex);
    }

    void SetPlayerController(const CUtlStringToken& keyToken, CBasePlayerController* pPlayerController)
    {
        vt::CallVirtual<void>(28, this, &keyToken, pPlayerController);
        //CallVFunc<void, 28U>(this, &keyToken, pPlayerController);
    }

    void SetPlayerPawn(const CUtlStringToken& keyToken, C_CSPlayerPawnBase* pPlayerPawn)
    {
        //   @XREF: "_pawn"
        vt::CallVirtual<void>(29, this, &keyToken, pPlayerPawn);
        //CallVFunc<void, 29U>(this, &keyToken, pPlayerPawn);
    }

public:
    std::byte pad0[0x60]; // 0x08
};
static_assert(sizeof(IGameEvent) == 0x68); // size verify   @ida: (CGameEventManager::CreateEvent) client.dll -> ["B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 23 4C 8B C5" + 0x1]
