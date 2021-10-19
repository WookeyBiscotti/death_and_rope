#include "main_menu.hpp"

#include <asset_cache.hpp>
#include <context.hpp>
#include <imgui_utils.hpp>
#include <imgui.h>

void MainMenu::onFrame()
{
    MIDDLE_WINDOW();
    ImGui::Begin("Main menu", nullptr, MENU_FLAGS);
    ImGui::Text("Main menu");
    if (ImGui::Button("Dev")) {
        context().nextScene = context().cache.scene("dev_menu");
    }
    if (ImGui::Button("Exit")) {
        context().isRuning = false;
    }
    ImGui::End();
}

bool MainMenu::active() const
{
    return _active;
}

void MainMenu::active(bool active)
{}