#include "sprites_view.hpp"

#include <context.hpp>
//
#include <archive.hpp>
#include <imgui_utils.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/scenes/scene_system.hpp>
//
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

SpritesView::SpritesView(Context& context): Scene(context) {
	auto files = context.systemRef<AssetCache>().sprites();
	for (const auto& f : files) {
		auto s = context.systemRef<AssetCache>().sprite(f);
		if (s) {
			_sprites.push_back(s);
		}
	}
}

void SpritesView::refresh() {
	_sprites.clear();

	auto files = context().systemRef<AssetCache>().sprites();
	for (const auto& f : files) {
		auto s = context().systemRef<AssetCache>().sprite(f);
		if (s) {
			_sprites.push_back(s);
		}
	}
}

void SpritesView::onFrame() {
	ImGui::Begin("Sprite view");
	if (ImGui::Button("Refresh")) {
		refresh();
	}
	if (ImGui::Button("Back")) {
		context().systemRef<SceneSystem>().next(context().systemRef<AssetCache>().scene("dev_menu"));
	}
	for (const auto& s : _sprites) {
		ImGui::TextUnformatted(s->name().c_str());
		ImGui::Image(s->sf());
		ImGui::Separator();
	}
	ImGui::End();
}
