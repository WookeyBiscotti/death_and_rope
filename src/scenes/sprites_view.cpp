#include "sprites_view.hpp"

#include <asset_cache.hpp>
#include <context.hpp>
#include <imgui_utils.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

SpritesView::SpritesView(Context& context)
    : Scene(context)
{
    auto files = context.cache.sprites();
    for (const auto& f : files) {
        auto s = context.cache.sprite(f);
        if (s) {
            _sprites.push_back(s);
        }
    }
}

void SpritesView::refresh()
{
    _sprites.clear();

    auto files = context().cache.sprites();
    for (const auto& f : files) {
        auto s = context().cache.sprite(f);
        if (s) {
            _sprites.push_back(s);
        }
    }
}

void SpritesView::onFrame()
{
    ImGui::Begin("Sprite view");
    if (ImGui::Button("Refresh")) {
        refresh();
    }
    if (ImGui::Button("Back")) {
        context().nextScene = context().cache.scene("dev_menu");
    }
    for (const auto& s : _sprites) {
        ImGui::TextUnformatted(s->name().c_str());
        ImGui::Image(s->sprite());
        ImGui::Separator();
    }
    ImGui::End();
}
