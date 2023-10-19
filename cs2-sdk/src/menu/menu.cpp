#include "pch.hpp"

#include <menu/menu.hpp>
#include <logger/logger.hpp>

#include <vars/vars.hpp>

#include <instance/instance.hpp>
#include <renderer/renderer.hpp>

#include <interfaces/inputsystem.hpp>
#include <sdl/sdl.hpp>

#include <imgui/imgui.h>
#include <config/config.hpp>

void CMenu::Render()
{
    if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
    {
        Toggle(!IsOpen());
    }
    else if (ImGui::IsKeyPressed(ImGuiKey_End, false))
    {
        return CInstance::Get().FreeLibrary();
    }

    RenderWatermark();
    RenderUI();
}

void CMenu::Shutdown()
{
    SDK_LOG_PROLOGUE();

    Toggle(false);
}

void CMenu::Toggle(bool state)
{
    if (!ImGui::GetCurrentContext())
    {
        return;
    }

    m_Open = state;

    auto inputSystem = CInputSystem::Get();
    if (inputSystem && inputSystem->IsRelativeMouseMode())
    {
        const ImVec2 screenCenter = ImGui::GetIO().DisplaySize / 2.f;

        sdl::SetRelativeMouseMode(!m_Open);
        sdl::SetWindowGrab(inputSystem->GetSDLWindow(), !m_Open);
        sdl::WarpMouseInWindow(nullptr, screenCenter.x, screenCenter.y);
    }
}

void CMenu::RenderWatermark()
{
    auto drawList = CRenderer::GetBackgroundDrawList();

    char framerate[128];
    snprintf(framerate, IM_ARRAYSIZE(framerate), "cs2-sdk v2\nFPS: %d",
        static_cast< int >(ImGui::GetIO().Framerate));

    drawList->AddText({ 17, 9 }, IM_COL32(0, 0, 0, 255), framerate);
    drawList->AddText({ 16, 8 }, IM_COL32(27, 227, 200, 255), framerate);
}

string DropDownPreviewTest(std::map<const char*, bool> dropDownSelectionMap)
{
    string endResult;
    auto pointInMap = dropDownSelectionMap.begin();

    int selectedItems = 0;
    while (pointInMap != dropDownSelectionMap.end())
    {
        //is hitbox selected?
        if (pointInMap->second)
        {
            if (selectedItems > 0)
            {
                endResult += ", ";
            }
            endResult += pointInMap->first;
            selectedItems++;
        }
        pointInMap++;
    }
    if (selectedItems == 0) return "-";

    return endResult;
}


string DropDownPreview(const char* names[], bool itemSelections[], int length)
{
    string endResult;
    int selectedItems = 0;
    for (int i = 0; i < length; i++)
    {
        if (itemSelections[i])
        {
            if (selectedItems > 0)
            {
                endResult += ", ";
            }
            endResult += names[i];
            selectedItems++;
        }
    }
    if (selectedItems == 0) return "-";

    return endResult;
}

void CMenu::RenderUI()
{
    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    if (!IsOpen())
    {
        return;
    }

    IO.ConfigFlags = ImGuiConfigFlags_None;

    auto inputSystem = CInputSystem::Get();
    if (inputSystem)
    {
        sdl::SetRelativeMouseMode(false);
        sdl::SetWindowGrab(inputSystem->GetSDLWindow(), false);
    }

    ImGui::SetNextWindowPos(IO.DisplaySize / 2.f, ImGuiCond_Once, { 0.5f, 0.5f });
    ImGui::Begin("cs2-sdk v2", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Aimbot");
    ImGui::BeginChild("Aimbot", ImVec2(m_WindowWidth / 2, 150), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Checkbox("Enable Aimbot", &g_Vars.m_Aimbot);


    if (ImGui::BeginCombo("##combo", DropDownPreviewTest(g_Vars.m_HitBoxes).c_str()))
    {
        for (auto& hitBox : g_Vars.m_HitBoxes)
        {
            bool selected = hitBox.second;
            if (ImGui::Selectable(hitBox.first, selected, ImGuiSelectableFlags_DontClosePopups))
            {
                hitBox.second = !hitBox.second;
            }
        }
        ImGui::EndCombo();
    }

    /*
    auto length = IM_ARRAYSIZE(g_Vars.m_HitBoxesName);
    if (ImGui::BeginCombo("##combo", DropDownPreview(g_Vars.m_HitBoxesName, g_Vars.m_SelectedHitBoxes, length).c_str()))
    {
        for (int n = 0; n < IM_ARRAYSIZE(g_Vars.m_HitBoxesName); n++)
        {
            bool is_selected = g_Vars.m_SelectedHitBoxes[n];
            if (ImGui::Selectable(g_Vars.m_HitBoxesName[n], is_selected, ImGuiSelectableFlags_DontClosePopups))
            {
                g_Vars.m_SelectedHitBoxes[n] = !g_Vars.m_SelectedHitBoxes[n];
            }
        }
        ImGui::EndCombo();
    }
    */

    ImGui::Checkbox("Auto Shoot", &g_Vars.m_AutoFire);
    ImGui::Checkbox("Silent", &g_Vars.m_SilentAim);
    ImGui::Text("FOV");
    ImGui::SliderInt("##FOV", &g_Vars.m_AimbotFov, 1, 180);
    ImGui::EndChild();

    ImGui::Text("Visuals");
    ImGui::BeginChild("Visuals", ImVec2(m_WindowWidth / 2, 325), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Checkbox("Enable", &g_Vars.m_EnableESP);
    ImGui::Checkbox("Players box", &g_Vars.m_PlayerBoxes);
    ImGui::Checkbox("Players name", &g_Vars.m_PlayerNames);
    ImGui::Checkbox("Weapon name", &g_Vars.m_WeaponName);
    ImGui::Checkbox("Player healthbar", &g_Vars.m_PlayerHealthBar);
    ImGui::Checkbox("Player armorbar ", &g_Vars.m_PlayerArmorBar);
    ImGui::SameLine(m_WindowWidth / 2 - 30, ImGui::GetStyle().ItemSpacing.x);
    ImGui::ColorEdit4("##ArmorColor", g_Vars.m_PlayerArmorColor, ImGuiColorEditFlags_NoInputs);

    ImGui::Checkbox("Player", &g_Vars.m_VisibleChams);
    ImGui::SameLine(m_WindowWidth / 2 - 30, ImGui::GetStyle().ItemSpacing.x);
    ImGui::ColorEdit4("##VisibleChams", g_Vars.m_PlayerVisibleChamsColor, ImGuiColorEditFlags_NoInputs);
    ImGui::Checkbox("Player Behind Wall", &g_Vars.m_InvisibleChams);
    ImGui::SameLine(m_WindowWidth / 2 - 30, ImGui::GetStyle().ItemSpacing.x);
    ImGui::ColorEdit4("##InvisibleChams", g_Vars.m_PlayerInvisChamsColor, ImGuiColorEditFlags_NoInputs);

    ImGui::Checkbox("Player Glow", &g_Vars.m_Glow);
    ImGui::SameLine(m_WindowWidth / 2 - 30, ImGui::GetStyle().ItemSpacing.x);
    ImGui::ColorEdit4("##Glow Color", g_Vars.m_GlowColor, ImGuiColorEditFlags_NoInputs);

    //ImGui::Hotkey(, ImVec2(2,2));
    ImGui::Checkbox("Weapons ESP", &g_Vars.m_WeaponESP);
    ImGui::Checkbox("Chickens ESP", &g_Vars.m_ChickenESP);
    ImGui::Checkbox("Others ESP", &g_Vars.m_OtherESP);
    ImGui::Checkbox("Three-dimensional boxes", &g_Vars.m_Use3DBoxes);
    ImGui::EndChild();

    ImGui::Text("Misc");
    ImGui::BeginChild("Misc", ImVec2(m_WindowWidth / 2, 100), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Checkbox("Automatic Jump", &g_Vars.m_Bhop);
    ImGui::Checkbox("Enable View Model Fov", &g_Vars.m_ViewModelFov);
    ImGui::SliderInt("##View Model Fov", &g_Vars.m_ViewModelFovSlider, 80, 110);

    ImGui::EndChild();


    if (ImGui::Button("Save Config", { m_WindowWidth, 0 })) Config::Get().SaveConfig();
    if (ImGui::Button("Load Config", { m_WindowWidth, 0 })) Config::Get().LoadConfig();


    if (ImGui::Button("Unload", { m_WindowWidth, 0 })) CInstance::Get().FreeLibrary();

    ImGui::End();
}
