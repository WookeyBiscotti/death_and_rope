#pragma once

#include <systems/assets/sprite.hpp>
#include <systems/scenes/scene.hpp>
//
#include <imgui.h>
//
#include <imfilebrowser.h>

class SpriteEditor: public Scene {
  public:
	explicit SpriteEditor(Context& context): Scene(context) {
		_fileDialog.SetTitle("Select texture");
		_fileDialog.SetTypeFilters({".png"});
	}

	void onFrame() override;

	bool active() const override;
	void active(bool active) override;

  private:
	bool _showSaveDialog = false;
	std::string _saveName;

	bool _active = false;
	std::shared_ptr<Sprite> _sprite;
	std::shared_ptr<Texture> _texture;
	ImGui::FileBrowser _fileDialog;
};