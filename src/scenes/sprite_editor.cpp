#include "sprite_editor.hpp"

#include <asset_cache.hpp>
#include <context.hpp>
#include <imgui_utils.hpp>
#include <imgui.h>

void SpriteEditor::onFrame()
{
    MIDDLE_WINDOW();
    ImGui::Begin("Sprite editor", nullptr, MENU_FLAGS);
    ImGui::Text("Sprite editor");
    if (ImGui::Button("Back")) {
        context().nextScene = context().cache.scene("dev_menu");
    }
    ImGui::End();
}

bool SpriteEditor::active() const
{
    return _active;
}

void SpriteEditor::active(bool active)
{}