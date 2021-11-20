#include "sprite_editor.hpp"

#include <common/imgui_utils.hpp>
#include <common/rect.hpp>
#include <common/types.hpp>
#include <engine/context.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/scenes/scene_system.hpp>
//
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

void SpriteEditor::onFrame() {
	if (_showSaveDialog) {
		ImGui::InputText("Asset name", &_saveName);
		if (ImGui::Button("Ok")) {
			_showSaveDialog = false;
			_sprite->name(_saveName);
			_sprite->saveToFile(context().systemRef<AssetCache>().spritesPath() / _saveName);
		}
		return;
	}

	ImGui::Begin("Sprite editor");
	ImGui::Text("Sprite editor");
	if (ImGui::Button("Open texture")) {
		_fileDialog.Open();
	}

	if (_texture && ImGui::Button("Save")) {
		_showSaveDialog = true;
		_saveName.clear();
	}
	if (ImGui::Button("Back")) {
		context().systemRef<SceneSystem>().findNext("dev_menu");
	}

	if (_fileDialog.HasSelected()) {
		auto texture = context().systemRef<AssetCache>().texture(_fileDialog.GetSelected().filename().string());
		if (texture) {
			_texture.reset();
			_texture = std::move(texture);
			if (!_sprite) {
				_sprite = std::make_shared<Sprite>("");
			}
			_sprite->_texture = _texture;
			_sprite->sf().setTexture(_texture->sf());
		}
		_fileDialog.ClearSelected();
	}

	_fileDialog.Display();

	ImGui::End();

	ImGui::Begin("Texture");
	if (_texture) {
		ImGui::Image(_texture->sf());
	}
	ImGui::End();

	ImGui::Begin("Sprite");

	if (_sprite) {
		ImGui::DragInt("x", &_sprite->_textureRect.left);
		ImGui::DragInt("y", &_sprite->_textureRect.top);

		ImGui::DragInt("width", &_sprite->_textureRect.width);
		ImGui::DragInt("height", &_sprite->_textureRect.height);
		float scale = _sprite->sf().getScale().x;
		ImGui::DragFloat("scale", &scale);
		_sprite->sf().setScale(scale, scale);

		_sprite->sf().setTextureRect(_sprite->_textureRect);
		_sprite->_textureRect = _sprite->_textureRect;

		ImGui::Image(_sprite->sf());
	}
	ImGui::End();
}

bool SpriteEditor::active() const {
	return _active;
}

void SpriteEditor::active(bool active) {
}