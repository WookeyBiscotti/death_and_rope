#include "main_menu.hpp"

#include "alch/common/imgui_utils.hpp"
#include "alch/engine/context.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/scenes/scene_system.hpp"

#include <cstddef>
#include <imgui.h>

using namespace al;

void MainMenu::onFrame() {
	MIDDLE_WINDOW();
	ImGui::Begin("Main menu", nullptr, MENU_FLAGS);
	ImGui::Text("Main menu");
	if (ImGui::Button("Dev")) {
		context().systemRef<SceneSystem>().findNext("dev_menu");
	}
	if (ImGui::Button("Exit")) {
		context().systemRef<SceneSystem>().exit();
	}
	ImGui::End();
}

bool MainMenu::active() const {
	return _active;
}

void MainMenu::active(bool active) {
}
