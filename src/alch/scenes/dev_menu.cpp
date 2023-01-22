#include "dev_menu.hpp"

#include "alch/common/imgui_utils.hpp"
#include "alch/engine/context.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/scenes/scene_system.hpp"

using namespace al;

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
