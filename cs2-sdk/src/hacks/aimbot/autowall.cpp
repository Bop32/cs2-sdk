#include <pch.hpp>
#include <hacks/aimbot\autowall.hpp>
#include <hacks/aimbot/aimbot.hpp>


bool PlayerHasArmor(C_CSPlayerPawnBase* player, int hitgroup)
{
    if (player->m_ArmorValue() <= 0) return false;

    switch (hitgroup)
    {
        case HITGROUP::HITGROUP_HEAD:
            return player->m_pItemServices()->m_bHasHelmet();
        case HITGROUP::HITGROUP_GENERIC:
        case HITGROUP::HITGROUP_CHEST:
        case HITGROUP::HITGROUP_STOMACH:
        case HITGROUP::HITGROUP_LEFTARM:
        case HITGROUP::HITGROUP_RIGHTARM:
            return true;
        default:
            return false;
    }

    return false;
}

void AutoWall::ScaleDamage(C_CSPlayerPawnBase* player, weapon::CCSWeaponBaseVData* weaponData, float& damage, int hitgroup)
{
    bool hasArmor = PlayerHasArmor(player, hitgroup);
    switch (hitgroup)
    {
        case HITGROUP::HITGROUP_HEAD:
            damage *= 4.f;
            break;
        case HITGROUP::HITGROUP_STOMACH:
            damage *= 1.25f;
            break;
        case HITGROUP::HITGROUP_LEFTLEG:
        case HITGROUP::HITGROUP_RIGHTLEG:
            damage *= 0.75f;
            break;
        default:
            break;
    }

    if (hasArmor)
    {
        float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weaponData->m_flArmorRatio() / 2.f;

        auto NewDamage = damage * armorRatio;

        if (((damage - (damage * armorRatio)) * (bonusValue * armorBonusRatio)) > player->m_ArmorValue())
            NewDamage = damage - (player->m_ArmorValue() / armorBonusRatio);

        damage = NewDamage;
    }
}

//Currently all this does is just check for the best hitbox not taking into account damage dropoff.
//Works for now as if you have multiple hitboxes selected it will go for the highest damage one
//Need to hook these two. LOOK AT TRACESHAPE FOR REFERENCE.
/*
    using SHandleBulletPenetration = bool(__fastcall*)(float&, int&, bool&, void*, FVector&, void*, float, float, bool, uint32, float, int&, FVector&, float, float, float&, uint32, void*);
    SHandleBulletPenetration HandleBulletPenetration = "E8 ? ? ? ? BA ? ? ? ? 48 8D 0D ? ? ? ? 0F B6 D8 E8 ? ? ? ? 48 85 C0 75 0B 48 8B 05 ? ? ? ? 48 8B 40 08 66 0F 6E 00" Follow JMP

    using SClipTraceToPlayers = void(__fastcall*)(const FVector&, const FVector&, STraceFilter*, SGameTrace*, float, float, float);
    SClipTraceToPlayers ClipTraceToPlayers = "E8 ? ? ? ? 0F B6 9E ? ? ? ? 80 FB 01" Follow JMP

*/
bool AutoWall::CanHit(C_CSPlayerPawnBase* player, weapon::CCSWeaponBaseVData* weaponInfo, int hitgroup, float fraction, float& damage)
{
    float maxRange = weaponInfo->m_flRange();
    float weaponDefaultDamage = weaponInfo->m_nDamage();
    float currentDistance = 0;  
    while (weaponDefaultDamage >= 1.f)
    {
        maxRange -= currentDistance;

        currentDistance += fraction * maxRange;

        //weaponDefaultDamage *= pow(weaponInfo->m_flRangeModifier(), currentDistance / 500.f);

        ScaleDamage(player, weaponInfo, weaponDefaultDamage, hitgroup);
        damage = weaponDefaultDamage;
        return true;
    }
    return false;
}

