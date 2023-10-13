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

CCachedPlayer::CCachedPlayer(CBaseHandle handle) : CCachedBaseEntity(handle) {}

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

void CCachedPlayer::RenderESP()
{
    CCSPlayerController* controller = Get<CCSPlayerController>();

    if (controller->m_iTeamNum() == CGameEntitySystem::GetLocalPlayerController()->m_iTeamNum() ||
        controller->IsWeapon()) return;

    if (g_Vars.m_Glow)
    {
        RenderGlow(controller->m_hPawn().Get());
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
        auto s = std::to_string(maxArmor);
        const char* armorValue = s.c_str();
        const ImVec2 startPoint = { min.x, max.y + 2 };
        const ImVec2 endPoint { max.x, max.y + 2 };
        float width = ((endPoint.x - startPoint.x) * maxArmor) / 100.f;

        drawList->AddLine(startPoint, endPoint, IM_COL32(0, 0, 0, 255), 3);
        if (maxArmor > 0)
        {
            drawList->AddLine({ startPoint.x + ( std::max )(1.f, width) , startPoint.y }, startPoint, IM_COL32(102, 190, 208, 255), 2);

            if (maxArmor <= 60)
            {
                ImVec2 fontSize = nameFont->CalcTextSizeA(nameFont->FontSize, FLT_MAX, 0, armorValue);
                drawList->AddText(nameFont, nameFont->FontSize, ImVec2 {startPoint.x + ( std::max )(1.f, width) - fontSize.x / 2, startPoint.y}, IM_COL32(255, 255, 255, 255), armorValue);
            }

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


    if (g_Vars.m_PlayerHealthBar)
    {
        const int clampedHp = std::min(controller->m_hPawn().Get()->m_iHealth(), 100u);

        const ImVec2 barMin = min - ImVec2 { 5, 0 };
        const ImVec2 barMax { min.x - 2, max.y };

        const float green = clampedHp * 2.f * 2.55f;
        const float greenClamped = ( std::min )(255.f, green);
        const float red = ( std::min )(255.f, 510 - green);

        const float height = ((barMax.y - barMin.y) * clampedHp) / 100.f;

        drawList->AddRectFilled(barMin - ImVec2 { 0, 1 }, barMax + ImVec2 { 0, 1 }, IM_COL32(0, 0, 0, 255));
        if (clampedHp > 0)
        {
            drawList->AddRectFilled({ barMin.x + 1, barMax.y - ( std::max )(1.f, height) }, { barMax.x - 1, barMax.y },
                IM_COL32(red, greenClamped, 0, 255));
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

    for (int i = 0; i < 15; ++i)
    {
        Vector rot1, rot2;
        ImVec2 Out1, Out2;
        Vector From;
        Vector To;

        controller->m_hPawn().Get()->GetBonePosition(bones[i][0], From, rot1);
        controller->m_hPawn().Get()->GetBonePosition(bones[i][1], To, rot1);

        if (CMath::Get().WorldToScreen(From, Out1) && CMath::Get().WorldToScreen(To, Out2))
        {
            drawList->AddLine(Out1, Out2, IM_COL32(255, 255, 255, 255));
        }
    }

    /*
    C_AttributeContainer* pAttributeContainer = controller->m_hPawn().Get()->m_pWeaponServices()->m_hActiveWeapon().Get()->m_AttributeManager();

    if (!pAttributeContainer) return;

    weapon::C_EconItemView* pItemView = pAttributeContainer->m_Item();

    if (!pItemView) return;

    weapon::CEconItemDefinition* pItemStaticData = pItemView->GetStaticData();

    if (!pItemStaticData) return;

    CLogger::Log("{}", pItemStaticData->GetSimpleWeaponName());
    */
    /*
    CGameSceneNode* gameSceneNode = controller->m_pGameSceneNode();

    if (!gameSceneNode) return;

    skeleton::CSkeletonInstance* skeleton_instance = gameSceneNode->GetSkeletonInstance();

    if (skeleton_instance == nullptr) return;

    CModelState model_state = skeleton_instance->m_modelState();

    CModel model = model_state.modelHandle;

    for (std::int32_t i = 0; i < model.BoneCount; ++i)
    {
        auto test = model.GetBoneParent(i);

        if (test == -1) continue;

        ImVec2 bone_screen_position;
        ImVec2 bone_screen_parent_position;

        CMath::Get().WorldToScreen(Vector(model_state.bones[i].position.x, model_state.bones[i].position.y, model_state.bones[i].position.z), bone_screen_position);
        CMath::Get().WorldToScreen(Vector(model_state.bones[test].position.x, model_state.bones[test].position.y, model_state.bones[test].position.z), bone_screen_position);
        drawList->AddLine(bone_screen_position, bone_screen_parent_position, ImColor(255,255,255,255));
    }
    */
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
    CCachedPlayer* localPlayer = CMatchCache::Get().GetLocalPlayer();
    if (!localPlayer) return true;

    static ConVar* mp_teammates_are_enemies = CCVar::Get()->GetCvarByName("mp_teammates_are_enemies");
    return mp_teammates_are_enemies->GetValue<bool>() ? true : GetTeam() != localPlayer->GetTeam();
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
