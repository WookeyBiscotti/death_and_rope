#include "editor.hpp"

#include <systems/group/group.hpp>
//
#include <imgui.h>
#include <spdlog/fmt/fmt.h>

void EditorScene::active(bool active) {
	if (!active) {
		_root.reset();
		return;
	}

	_root = std::make_unique<Entity>(context());
};

bool EditorScene::drawNode(Entity& entity, Entity* selected) {
	if (auto g = entity.get<Group>()) {
		if (ImGui::TreeNode(fmt::format("Node: {}", (void*)&entity).c_str())) {
			ImGui::SameLine();
			ImGui::PushID(&entity);
			if (ImGui::SmallButton("+")) {
				g->create();
			};
			if (&entity != _root.get() && (ImGui::SameLine(), ImGui::SmallButton("-"))) {
				ImGui::PopID();
				ImGui::TreePop();
				return true;
			}
			ImGui::PopID();
			for (auto& c : g->childs()) {
				if (drawNode(*c, selected)) {
					g->remove(c);
					break;
				};
			}
			ImGui::TreePop();
		}
	} else {
		ImGui::Text("Node: %p", &entity);
		ImGui::SameLine();
		ImGui::PushID(&entity);
		if (ImGui::SmallButton("+")) {
			entity.add<Group>().ref<Group>().create();
		}
		if (&entity != _root.get() && (ImGui::SameLine(), ImGui::SmallButton("-"))) {
			ImGui::PopID();
			return true;
		}
		ImGui::PopID();
	}

	return false;
}

void EditorScene::onFrame() {
	ImGui::Begin("Entities");

	drawNode(*_root, _selected);

	// if (ImGui::TreeNode(_root.get(), "Root node: %p", _root.get())) {
	// 	static int selection_mask = (1 << 2);
	// 	int node_clicked = -1;
	// 	for (int i = 0; i < 6; i++) {
	// 		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
	// 		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter
	// 		// selection.
	// 		ImGuiTreeNodeFlags node_flags{}; // = base_flags;
	// 		const bool is_selected = (selection_mask & (1 << i)) != 0;
	// 		if (is_selected)
	// 			node_flags |= ImGuiTreeNodeFlags_Selected;
	// 		if (i < 3) {
	// 			// Items 0..2 are Tree Node
	// 			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
	// 			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	// 				node_clicked = i;
	// 			// if (test_drag_and_drop && ImGui::BeginDragDropSource()) {
	// 			// 	ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
	// 			// 	ImGui::Text("This is a drag and drop source");
	// 			// 	ImGui::EndDragDropSource();
	// 			// }
	// 			if (node_open) {
	// 				ImGui::BulletText("Blah blah\nBlah Blah");
	// 				ImGui::TreePop();
	// 			}
	// 		} else {
	// 			// Items 3..5 are Tree Leaves
	// 			// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
	// 			// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
	// 			node_flags |=
	// 			    ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
	// 			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
	// 			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	// 				node_clicked = i;
	// 			// if (test_drag_and_drop && ImGui::BeginDragDropSource()) {
	// 			// 	ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
	// 			// 	ImGui::Text("This is a drag and drop source");
	// 			// 	ImGui::EndDragDropSource();
	// 			// }
	// 		}
	// 	}
	// 	if (node_clicked != -1) {
	// 		// Update selection state
	// 		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
	// 		if (ImGui::GetIO().KeyCtrl)
	// 			selection_mask ^= (1 << node_clicked); // CTRL+click to toggle
	// 		else // if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want
	// 		     // to preserve selection when clicking on item that is part of the selection
	// 			selection_mask = (1 << node_clicked); // Click to single-select
	// 	}
	// 	// if (align_label_with_current_x_position)
	// 	// 	ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	// 	ImGui::TreePop();
	// }
	ImGui::End();
}