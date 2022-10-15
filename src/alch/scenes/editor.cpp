#include "alch/systems/group/group.hpp"
#include "editor.hpp"
//
#include <imgui.h>
#include <spdlog/fmt/fmt.h>

using namespace al;

void EditorScene::active(bool active) {
	if (!active) {
		_root.reset();
		return;
	}

	_root = std::make_unique<Entity>(context());
};

bool EditorScene::drawNode(Entity& entity, Entity*& selected) {
	auto g = entity.get<Group>();

	int flags = g ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_Leaf;
	if (&entity == selected) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	const bool openned = ImGui::TreeNodeEx(fmt::format("Node: {}", (void*)&entity).c_str(), flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		selected = &entity;
	}
	ImGui::SameLine();
	ImGui::PushID(&entity);
	if (ImGui::SmallButton("Child+")) {
		if (g) {
			g->create();
		} else {
			entity.addChain<Group>().ref<Group>().create();
		}
	}
	if (&entity != _root.get() && (ImGui::SameLine(), ImGui::SmallButton("Delete"))) {
		ImGui::PopID();
		if (openned) {
			ImGui::TreePop();
		}

		return true;
	}
	if (g) {
		ImGui::SameLine();
		bool move = g->moveChilds();
		if (ImGui::Checkbox("Move childs", &move)) {
			g->moveChilds(move);
		}
	}

	ImGui::PopID();

	if (g && openned) {
		for (auto& c : g->childs()) {
			if (drawNode(*c, selected)) {
				g->remove(c);
				break;
			};
		}
	}
	if (openned) {
		ImGui::TreePop();
	}

	return false;
}

void EditorScene::onFrame() {
	ImGui::Begin("Entities");
	drawNode(*_root, _selected);
	ImGui::End();
}
