#include "pch.hpp"

#include <cache/entities/player.hpp>
#include <cache/cache.hpp>

#include <renderer/renderer.hpp>
#include <vars/vars.hpp>

#include <bindings/playercontroller.hpp>
#include <bindings/playerpawn.hpp>

#include <interfaces/engineclient.hpp>
#include <interfaces/cvar.hpp>

#include <imgui/imgui_internal.h>
#include <interfaces/gameentitysystem.hpp>
#include <globals/globals.hpp>
#include <logger/logger.hpp>
#include <fonts/fonts.hpp>
#include <interfaces/CLocalize.hpp>
#include <bonesystem/bonesystem.hpp>

using namespace fonts;

bool CCachedPlayer::CanDrawESP()
{
    CCSPlayerController* controller = Get<CCSPlayerController>();
    if (!controller || !controller->m_bPawnIsAlive())
    {
        return false;
    }

    C_CSPlayerPawnBase* pawn = controller->m_hPawn().Get();
    if (!pawn || pawn->IsObserverPawn())
    {
        return false;
    }

    return true;
}

void RenderGlow(C_CSPlayerPawnBase* pawn)
{
    pawn->m_Glow().m_bGlowing() = true;
    pawn->m_Glow().m_iGlowType() = 3;
    pawn->m_Glow().m_glowColorOverride() = IM_COL32(g_Vars.m_GlowColor[0] * 255,
        g_Vars.m_GlowColor[1] * 255, g_Vars.m_GlowColor[2] * 255, g_Vars.m_GlowColor[3] * 255);
}

void RenderSkeleton(C_CSPlayerPawnBase* pawn, ImDrawList* drawList) noexcept
{
    CGameSceneNode* gameSceneNode = pawn->m_pGameSceneNode();

    if (!gameSceneNode) return;

    CSkeletonInstance* skeleton_instance = gameSceneNode->GetSkeletonInstance();

    if (skeleton_instance == nullptr) return;

    CModelState model_state = skeleton_instance->m_modelState();

    const CStrongHandle<CModel> model = model_state.modelHandle;

    if (!model.is_valid()) return;

    for (std::int32_t i = 0; i < model->BoneCount; ++i)
    {
        auto flag = model->GetBoneFlag(i);

        bool hasFlag = flag & 0x100;

        if (!hasFlag) continue;

        auto boneParentIndex = model->GetBoneParent(i);

        if (boneParentIndex == -1) continue;

        ImVec2 bone_screen_position;
        ImVec2 bone_screen_parent_position;

        Vector firstPoint = Vector(
            model_state.bones[i].position.x,
            model_state.bones[i].position.y,
            model_state.bones[i].position.z);
        Vector secondPoint = Vector(
            model_state.bones[boneParentIndex].position.x,
            model_state.bones[boneParentIndex].position.y,
            model_state.bones[boneParentIndex].position.z);

        if (CMath::Get().WorldToScreen(firstPoint, bone_screen_position) && 
            CMath::Get().WorldToScreen(secondPoint, bone_screen_parent_position))
        {
            drawList->AddLine(bone_screen_position, bone_screen_parent_position, ImColor(255, 255, 255, 255));
        }
    }
}

void CCachedPlayer::RenderESP()
{
    CCSPlayerController* controller = Get<CCSPlayerController>();

    auto localPlayerController = CGameEntitySystem::GetLocalPlayerController();

    if (!controller->m_bPawnIsAlive() || controller->m_iTeamNum() == localPlayerController->m_iTeamNum() ||
        controller->IsWeapon() || controller->m_pGameSceneNode()->m_bDormant()) return;

    C_CSPlayerPawnBase* pawn = controller->m_hPawn().Get();

    if (g_Vars.m_Glow)
    {
        RenderGlow(pawn);
    }

    if (g_Vars.m_PlayerBoxes)
    {
        UpdateBoxColor();
        CCachedBaseEntity::RenderESP();
    }

    auto drawList = CRenderer::GetBackgroundDrawList();

    const ImVec2& min = m_BBox.m_Mins;
    const ImVec2& max = m_BBox.m_Maxs;

    if (g_Vars.m_PlayerArmorBar)
    {
        // p_min = upper-left, p_max = lower-right

        int maxArmor = std::min(controller->m_hPawn().Get()->m_ArmorValue(), 100u);
        const ImVec2 startPoint = { min.x, max.y + 2 };
        const ImVec2 endPoint { max.x, max.y + 2 };
        float width = ((endPoint.x - startPoint.x) * maxArmor) / 100.f;

        if (maxArmor > 0)
        {
            drawList->AddLine(startPoint, endPoint, IM_COL32(0, 0, 0, 255), 3);
            drawList->AddLine({ startPoint.x + ( std::max )(1.f, width) , startPoint.y }, startPoint,
                IM_COL32(g_Vars.m_PlayerArmorColor[0] * 255,
                g_Vars.m_PlayerArmorColor[1] * 255,
                g_Vars.m_PlayerArmorColor[2] * 255,
                g_Vars.m_PlayerArmorColor[3] * 255), 2);
        }
    }

    if (g_Vars.m_PlayerNames)
    {
        const char* playerName = controller->m_sSanitizedPlayerName();
        if (playerName)
        {
            const ImVec2 textSize = ImGui::CalcTextSize(playerName);
            const ImVec2 textPos = { (min.x + max.x) / 2.f - textSize.x / 2, min.y - textSize.y - 2.f };
            drawList->AddText(textPos + ImVec2 { 1, 1 }, IM_COL32(0, 0, 0, 255), playerName);
            drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), playerName);
        }
    }
    if (g_Vars.m_WeaponName)
    {
        //This is needed here otherwise it will crash if the enemy dies
        if (pawn->m_iHealth() > 0)
        {
            C_AttributeContainer* pAttributeContainer = pawn->m_pWeaponServices()->m_hActiveWeapon().Get()->m_AttributeManager();

            if (!pAttributeContainer) return;

            C_EconItemView* pItemView = pAttributeContainer->m_Item();

            if (!pItemView) return;

            CEconItemDefinition* pItemStaticData = pItemView->GetStaticData();

            if (!pItemStaticData) return;

            auto weaponName = CLocalize::Get()->FindSafe(pItemStaticData->m_pszItemBaseName);

            const ImVec2 textSize = ImGui::CalcTextSize(weaponName);
            const ImVec2 textPos = { (min.x + max.x) / 2.f - textSize.x / 2, max.y + textSize.y / 2 - 3.5f };
            drawList->AddText(textPos + ImVec2 { 1, 1 }, IM_COL32(0, 0, 0, 255), weaponName);
            drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), weaponName);
        }
    }


    if (g_Vars.m_PlayerHealthBar)
    {
        const int clampedHp = std::min(controller->m_hPawn().Get()->m_iHealth(), 100u);

        const ImVec2 barMin = min - ImVec2 { 5, 0 };
        const ImVec2 barMax { min.x - 2, max.y };

        const float green = clampedHp * 2.f * 2.55f;
        const float greenClamped = ( std::min )(255.f, green);
        const float red = ( std::min )(255.f, 510 - green);

        const float height = ((barMax.y - barMin.y) * clampedHp) / 100.f;

        if (clampedHp > 0)
        {
            drawList->AddRectFilled(barMin - ImVec2 { 0, 1 }, barMax + ImVec2 { 0, 1 }, IM_COL32(0, 0, 0, 255));
            drawList->AddRectFilled({ barMin.x + 1, barMax.y - ( std::max )(1.f, height) }, { barMax.x - 1, barMax.y },
                IM_COL32(red, greenClamped, 0, 255));
        }
        if (clampedHp <= 45)
        {
            auto s = std::to_string(clampedHp);
            const char* health = s.c_str();
            int const fontSize = 12;
            ImVec2 textSize = nameFont->CalcTextSizeA(fontSize, FLT_MAX, 0, health);
            ImVec2 textPosition = { barMin.x - textSize.x, barMax.y - ( std::max )(1.f, height) - textSize.y / 2 };
            drawList->AddText(nameFont, fontSize,
                ImVec2(textPosition + ImVec2 { 1,1 }),
                IM_COL32(0, 0, 0, 255), health);
            drawList->AddText(nameFont, fontSize,
                textPosition,
                IM_COL32(255, 255, 255, 255), health);
        }
    }

    const int bones[][2] =
    {
        { 27, 26 },{ 26, 25 },{ 25, 0 }, //right leg
        { 24, 23 },{ 23, 22 },{ 22, 0 }, //left leg
        { 5, 8 },{ 8, 9 },{ 9, 10 }, //left arm
        { 5, 13 },{ 13, 14 },{ 14, 15 }, //right arm
        { 0, 2 },{ 2, 5 },{ 5, 6 } //spine
    };

    if (g_Vars.m_SkeletonEsp)
    {
        RenderSkeleton(pawn, drawList);
    }

}

void CCachedPlayer::UpdateESP()
{
    CCSPlayerController* controller = Get<CCSPlayerController>();
    C_BaseEntity* pawn = controller->m_hPawn().Get();
    if (!pawn->CalculateBBoxByCollision(m_BBox))
    {
        return ResetESP();
    }
}

uint8_t CCachedPlayer::GetTeam()
{
    CCSPlayerController* controller = Get<CCSPlayerController>();
    if (!controller) return 0;

    C_CSPlayerPawnBase* pawn = controller->m_hPawn().Get();
    if (!pawn) return 0;

    return pawn->m_iTeamNum();
}

bool CCachedPlayer::IsEnemyWithLocalPlayer()
{
    CCachedPlayer* localPlayerController = CMatchCache::Get().GetLocalPlayer();
    if (!localPlayerController) return true;

    static ConVar* mp_teammates_are_enemies = CCVar::Get()->GetCvarByName("mp_teammates_are_enemies");
    return mp_teammates_are_enemies->GetValue<bool>() ? true : GetTeam() != localPlayerController->GetTeam();
}

bool CCachedPlayer::IsLocalPlayer() { return m_Handle.GetEntryIndex() == CEngineClient::Get()->GetLocalPlayer(); }

void CCachedPlayer::UpdateBoxColor()
{
    m_BoxColor = [this]()
        {
            if (IsLocalPlayer())
            {
                return IM_COL32(52, 131, 235, 255);
            }
            else if (IsEnemyWithLocalPlayer())
            {
                return IM_COL32(255, 0, 0, 255);
            }

            return IM_COL32(0, 255, 0, 255);
        }();
}
