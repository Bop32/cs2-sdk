#include "pch.hpp"

#include <renderer/renderer.hpp>
#include <logger/logger.hpp>

#include <hacks/esp/esp.hpp>

#include <menu/menu.hpp>

#include <imgui/imgui.h>
#include <fonts/fonts.hpp>


void CRenderer::Initialize()
{
    SDK_LOG_PROLOGUE();
    ImGuiIO& io = ImGui::GetIO();
    fonts::nameFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahomabd.ttf", 13.f);
}

void CRenderer::NewFrame()
{
    m_BackgroundDrawList = ImGui::GetBackgroundDrawList();

    CMenu::Get().Render();
    CESP::Get().Render();
}
