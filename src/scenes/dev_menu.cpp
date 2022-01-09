#include "dev_menu.hpp"

#include <common/imgui_utils.hpp>
#include <engine/context.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/scenes/scene_system.hpp>

void DevMenu::onFrame() {
	MIDDLE_WINDOW();
	ImGui::Begin("Dev menu", nullptr, MENU_FLAGS);
	ImGui::Text("Dev menu");
	if (ImGui::Button("Sprite editor")) {
		context().systemRef<SceneSystem>().findNext("sprite_editor");
	}
	if (ImGui::Button("Sprite view")) {
		context().systemRef<SceneSystem>().findNext("sprites_view");
	}
	if (ImGui::Button("World editor")) {
		context().systemRef<SceneSystem>().findNext("world_editor");
	}
	if (ImGui::Button("Test scene")) {
		context().systemRef<SceneSystem>().findNext("test");
	}
	if (ImGui::Button("Test physics scene")) {
		context().systemRef<SceneSystem>().findNext("test_physics");
	}
	if (ImGui::Button("Back")) {
		context().systemRef<SceneSystem>().findNext("main_menu");
	}
	ImGui::End();
}

bool DevMenu::active() const {
	return _active;
}

void DevMenu::active(bool active) {
}