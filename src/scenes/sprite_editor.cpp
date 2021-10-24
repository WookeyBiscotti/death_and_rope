#include "sprite_editor.hpp"

#include <asset_cache.hpp>
#include <context.hpp>
#include <imgui_utils.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

void SpriteEditor::onFrame()
{
    if (_showSaveDialog) {
        ImGui::InputText("Asset name", &_saveName);
        if (ImGui::Button("Ok")) {
            _showSaveDialog = false;
            _sprite->name(_saveName);
            _sprite->saveToFile(context().cache.defaultSpritePath() + _saveName);
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
        context().nextScene = context().cache.scene("dev_menu");
    }

    if (_fileDialog.HasSelected()) {
        auto texture = context().cache.texture(_fileDialog.GetSelected().filename().string());
        if (texture) {
            _texture.reset();
            _texture = std::move(texture);
            if (!_sprite) {
                _sprite = std::make_shared<Sprite>("");
            }
            _sprite->_texture = _texture;
            _sprite->sprite().setTexture(_texture->texture());
        }
        _fileDialog.ClearSelected();
    }

    _fileDialog.Display();

    ImGui::End();

    ImGui::Begin("Texture");
    if (_texture) {
        ImGui::Image(_texture->texture());
    }
    ImGui::End();

    ImGui::Begin("Sprite");

    if (_sprite) {
        ImGui::DragInt("x", &_sprite->_textureRect.left);
        ImGui::DragInt("y", &_sprite->_textureRect.top);

        ImGui::DragInt("width", &_sprite->_textureRect.width);
        ImGui::DragInt("height", &_sprite->_textureRect.height);
        float scale = _sprite->sprite().getScale().x;
        ImGui::DragFloat("scale", &scale);
        _sprite->sprite().setScale(scale, scale);

        _sprite->sprite().setTextureRect(_sprite->_textureRect);
        _sprite->_textureRect = _sprite->_textureRect;

        ImGui::Image(_sprite->sprite());
    }
    ImGui::End();
}

bool SpriteEditor::active() const
{
    return _active;
}

void SpriteEditor::active(bool active)
{}