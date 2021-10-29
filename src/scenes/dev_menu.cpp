#include "dev_menu.hpp"

#include <asset_cache.hpp>
#include <context.hpp>
#include <imgui_utils.hpp>

void DevMenu::onFrame()
{
    MIDDLE_WINDOW();
    ImGui::Begin("Dev menu", nullptr, MENU_FLAGS);
    ImGui::Text("Dev menu");
    if (ImGui::Button("Sprite editor")) {
        context().nextScene = context().cache.scene("sprite_editor");
    }
    if (ImGui::Button("Sprite view")) {
        context().nextScene = context().cache.scene("sprites_view");
    }
    if (ImGui::Button("World editor")) {
        context().nextScene = context().cache.scene("world_editor");
    }
    if (ImGui::Button("Test scene")) {
        context().nextScene = context().cache.scene("test_scene");
    }
    if (ImGui::Button("Back")) {
        context().nextScene = context().cache.scene("main_menu");
    }
    ImGui::End();
}

bool DevMenu::active() const
{
    return _active;
}

void DevMenu::active(bool active)
{}