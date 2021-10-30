#include "dev_menu.hpp"

#include <context.hpp>
#include <imgui_utils.hpp>
#include <systems/assets/asset_cache.hpp>

void DevMenu::onFrame()
{
    MIDDLE_WINDOW();
    ImGui::Begin("Dev menu", nullptr, MENU_FLAGS);
    ImGui::Text("Dev menu");
    if (ImGui::Button("Sprite editor")) {
        context().nextScene = context().systemRef<AssetCache>().scene("sprite_editor");
    }
    if (ImGui::Button("Sprite view")) {
        context().nextScene = context().systemRef<AssetCache>().scene("sprites_view");
    }
    if (ImGui::Button("World editor")) {
        context().nextScene = context().systemRef<AssetCache>().scene("world_editor");
    }
    if (ImGui::Button("Test scene")) {
        context().nextScene = context().systemRef<AssetCache>().scene("test_scene");
    }
    if (ImGui::Button("Back")) {
        context().nextScene = context().systemRef<AssetCache>().scene("main_menu");
    }
    ImGui::End();
}

bool DevMenu::active() const
{
    return _active;
}

void DevMenu::active(bool active)
{}