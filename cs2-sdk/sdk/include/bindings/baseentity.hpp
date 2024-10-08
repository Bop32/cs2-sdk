#pragma once

#include <bindings/entityinstance.hpp>
#include <bindings/gamescenenode.hpp>

#include <bindings/collisionproperty.hpp>
#include <bindings/hitbox.hpp>
#include <signatures/signatures.hpp>
#include <hacks/Glow/CGlowProperty.hpp>
#include <bindings/c_player_cameraservices.hpp>
#include <weapon/itemservices.hpp>

struct BBox_t;

class C_BaseEntity : public CEntityInstance
{
public:
    bool IsPlayerController();
    bool IsWeapon();
    bool IsProjectile();
    bool IsPlantedC4();
    bool IsChicken();

    bool CalculateBBoxByCollision(BBox_t& out);
    bool CalculateBBoxByHitbox(BBox_t& out, bool transformAABB = false);

    CHitBoxSet* GetHitboxSet(int i);
    int HitboxToWorldTransforms(CHitBoxSet* hitBoxSet, CTransform* hitboxToWorld);

    SCHEMA(CGameSceneNode*, m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode");
    SCHEMA(CBaseHandle, m_hOwnerEntity, "C_BaseEntity", "m_hOwnerEntity");
    SCHEMA(CCollisionProperty*, m_pCollision, "C_BaseEntity", "m_pCollision");
    SCHEMA(uint8_t, m_iTeamNum, "C_BaseEntity", "m_iTeamNum");
    SCHEMA(Vector, m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset");
    SCHEMA(uint32_t, m_iFlags, "C_BaseEntity", "m_fFlags");
    SCHEMA(CGlowProperty, m_Glow, "C_BaseModelEntity", "m_Glow");
    SCHEMA(CUtlVector<Vector>, m_aimPunchCache, "C_CSPlayerPawn", "m_aimPunchCache");
    SCHEMA(Vector, m_aimPunchAngle, "C_CSPlayerPawn", "m_aimPunchAngle");
    SCHEMA(uint32_t, m_iHealth, "C_BaseEntity", "m_iHealth");
    SCHEMA(uint32_t, m_ArmorValue, "C_CSPlayerPawnBase", "m_ArmorValue");
    SCHEMA(CPlayer_CameraServices*, m_pCameraServices, "C_BasePlayerPawn", "m_pCameraServices");
    SCHEMA(CPlayer_ItemServices*, m_pItemServices, "C_BasePlayerPawn", "m_pItemServices");
    SCHEMA(uint32_t, m_nTickBase, "CBasePlayerController", "m_nTickBase");
    SCHEMA(uint32_t, m_MoveType, "C_BaseEntity", "m_MoveType");
    SCHEMA(Vector, m_vecVelocity, "C_BaseEntity", "m_vecVelocity");



    Vector GetEyePosition()
    {
        return m_pGameSceneNode()->m_vecAbsOrigin() + m_vecViewOffset();
    }

    void GetBonePosition(uint32_t bone_index, Vector& position, Vector& rotation)
    {
        using function_t = void(__fastcall*)(void*, uint32_t, Vector&, Vector&);
        static function_t fn = reinterpret_cast< function_t >(signatures::GetBonePosition.GetPtrAs<void*>());

        fn(this, bone_index, position, rotation);
    }
};

enum flags_t : uint32_t
{
    FL_ONGROUND = (1 << 0),
    FL_DUCKING = (1 << 1),
    FL_WATERJUMP = (1 << 2),
    FL_ONTRAIN = (1 << 3),
    FL_INRAIN = (1 << 4),
    FL_FROZEN = (1 << 5),
    FL_ATCONTROLS = (1 << 6),
    FL_CLIENT = (1 << 7),
    FL_FAKECLIENT = (1 << 8),
    FL_INWATER = (1 << 9),
    FL_FLY = (1 << 10),
    FL_SWIM = (1 << 11),
    FL_CONVEYOR = (1 << 12),
    FL_NPC = (1 << 13),
    FL_GODMODE = (1 << 14),
    FL_NOTARGET = (1 << 15),
    FL_AIMTARGET = (1 << 16),
    FL_PARTIALGROUND = (1 << 17),
    FL_STATICPROP = (1 << 18),
    FL_GRAPHED = (1 << 19),
    FL_GRENADE = (1 << 20),
    FL_STEPMOVEMENT = (1 << 21),
    FL_DONTTOUCH = (1 << 22),
    FL_BASEVELOCITY = (1 << 23),
    FL_WORLDBRUSH = (1 << 24),
    FL_OBJECT = (1 << 25),
    FL_KILLME = (1 << 26),
    FL_ONFIRE = (1 << 27),
    FL_DISSOLVING = (1 << 28),
    FL_TRANSRAGDOLL = (1 << 29),
    FL_UNBLOCKABLE_BY_PLAYER = (1 << 30)
};

enum movetype_t : std::uint32_t
{
    MOVETYPE_NONE = 0,
    MOVETYPE_ISOMETRIC,
    MOVETYPE_WALK,
    MOVETYPE_STEP,
    MOVETYPE_FLY,
    MOVETYPE_FLYGRAVITY,
    MOVETYPE_VPHYSICS,
    MOVETYPE_PUSH,
    MOVETYPE_NOCLIP,
    MOVETYPE_LADDER,
    MOVETYPE_OBSERVER,
    MOVETYPE_CUSTOM,
    MOVETYPE_LAST = MOVETYPE_CUSTOM,
    MOVETYPE_MAX_BITS = 4
};


