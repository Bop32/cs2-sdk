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
#include <log/log.hpp>
#include <globals/globals.hpp>

using namespace globals;
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
    RenderLogs();
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
    return;

}

void CMenu::RenderLogs()
{
    auto drawList = CRenderer::GetBackgroundDrawList();

    notify.NotifyLogic();
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
    ImGui::BeginChild("Aimbot", ImVec2(m_WindowWidth / 2, 225), true, ImGuiWindowFlags_NoScrollbar);
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

    ImGui::SliderInt("##Minimum Damage", &g_Vars.m_MinimumDamage, 1, 100);
    ImGui::Checkbox("Minimum hit chance", &g_Vars.m_HitChance);
    if (g_Vars.m_HitChance)
    {
        ImGui::SliderInt("##HitChance", &g_Vars.m_HitChanceValue, 1, 100);
    }

    ImGui::Checkbox("Auto Shoot", &g_Vars.m_AutoFire);
    ImGui::Checkbox("Silent", &g_Vars.m_SilentAim);
    ImGui::Checkbox("Prefer body aim on peek", &g_Vars.m_PreferBodyAim);
    ImGui::Text("FOV");
    ImGui::SliderInt("##FOV", &g_Vars.m_AimbotFov, 1, 180);
    ImGui::EndChild();

    ImGui::Text("Visuals");
    ImGui::BeginChild("Visuals", ImVec2(m_WindowWidth / 2, 325), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Checkbox("Enable", &g_Vars.m_EnableESP);
    ImGui::Checkbox("Players box", &g_Vars.m_PlayerBoxes);
    ImGui::Checkbox("Players name", &g_Vars.m_PlayerNames);
    ImGui::Checkbox("Player Skeleton", &g_Vars.m_SkeletonEsp);
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

    if (ImGui::BeginCombo("##ChamsType", DropDownPreviewTest(g_Vars.m_ChamsType).c_str()))
    {
        int selectedIndex = 0;
        for (auto& chamsType : g_Vars.m_ChamsType)
        {
            bool selected = chamsType.second;
            if (ImGui::Selectable(chamsType.first, selected))
            {
                chamsType.second = !chamsType.second;
                int index = 0;
                for (auto& removeChamsType : g_Vars.m_ChamsType)
                {
                    if (index != selectedIndex && removeChamsType.second)
                    {
                        removeChamsType.second = false;
                        ImGui::Selectable(chamsType.first, false);
                    }
                    index++;
                }
                break;
            }
            selectedIndex++;
        }
        ImGui::EndCombo();
    }



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
    ImGui::Checkbox("Third Person", &g_Vars.m_Thirdperson);
    ImGui::Checkbox("Automatic Jump", &g_Vars.m_Bhop);
    ImGui::Checkbox("Enable View Model Fov", &g_Vars.m_ViewModelFov);
    ImGui::SliderInt("##View Model Fov", &g_Vars.m_ViewModelFovSlider, 80, 110);

    ImGui::EndChild();


    if (ImGui::Button("Save Config", { m_WindowWidth, 0 })) Config::Get().SaveConfig();
    if (ImGui::Button("Load Config", { m_WindowWidth, 0 })) Config::Get().LoadConfig();


    if (ImGui::Button("Unload", { m_WindowWidth, 0 })) CInstance::Get().FreeLibrary();

    ImGui::End();
}
