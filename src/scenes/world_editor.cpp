#include "world_editor.hpp"

#include <common/imgui_utils.hpp>
#include <config.hpp>
#include <engine/context.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/render/render.hpp>
#include <systems/scenes/scene_system.hpp>
#include <systems/window/inputs.hpp>
#include <tile_utils.hpp>
//
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

WorldEditor::WorldEditor(Context& context): Scene(context) {
	_view = context.systemRef<Render>().target().getView();
}

void WorldEditor::onEvent(const sf::Event& e) {
	if (e.type == sf::Event::MouseButtonPressed) {
		const auto p = context().systemRef<Render>().target().mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
		paintPoint(p);
	}
}

void WorldEditor::paintPoint(const Vector2f& p) {
	auto& tile = _world.tileGetOrCreate(p);
	tile.typeId = _currentTileType;
}

void WorldEditor::onFrame() {
	if (_showSaveDialog) {
		ImGui::InputText("World name", &_saveName);
		if (ImGui::Button("Ok")) {
			_showSaveDialog = false;
			_world.saveToDir(context().systemRef<AssetCache>().worldsPath() / _saveName);
		}
		if (ImGui::Button("Cancel")) {
			_showSaveDialog = false;
		}
		return;
	}

	if (_showLoadDialog) {
		ImGui::Begin("Load");
		for (const auto& name : _worldsList) {
			ImGui::TextUnformatted(name.c_str());
			ImGui::SameLine();
			ImGui::PushID(&name);
			if (ImGui::Button("Ok")) {
				_showLoadDialog = false;
				_world.clear();
				_world.loadFromDir(context().systemRef<AssetCache>().worldsPath() / name);
			}
			ImGui::PopID();
			ImGui::Separator();
		}

		if (ImGui::Button("Cancel")) {
			_showLoadDialog = false;
		}
		ImGui::End();
		return;
	}

	ImGui::Begin("World editor");
	if (ImGui::Button("Save")) {
		_showSaveDialog = true;
	}
	if (ImGui::Button("Load")) {
		_worldsList = context().systemRef<AssetCache>().worlds();
		_showLoadDialog = true;
	}
	if (ImGui::Button("Back")) {
		context().systemRef<SceneSystem>().next(context().systemRef<AssetCache>().scene("dev_menu"));
	}
	ImGui::End();

	ImGui::Begin("Paint tool");
	ImGui::InputInt("Type", &_currentTileType);
	if (_currentTileType < 0) {
		_currentTileType = 0;
	}
	ImGui::End();

	context().systemRef<Render>().target().setView(_view);
	{
		sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(-1024, 0), sf::Color(0, 0, 255, 100)),
		    sf::Vertex(sf::Vector2f(1024, 0), sf::Color(0, 0, 255, 100))};
		sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(0, -1024), sf::Color(0, 0, 255, 100)),
		    sf::Vertex(sf::Vector2f(0, 1024), sf::Color(0, 0, 255, 100))};

		context().systemRef<Render>().target().draw(line1, 2, sf::Lines);
		context().systemRef<Render>().target().draw(line2, 2, sf::Lines);
	}
	{
		const auto l = _view.getCenter() - 0.5f * _view.getSize();
		const auto h = _view.getCenter() + 0.5f * _view.getSize();
		for (auto x = TILE_SIZE * int(l.x / TILE_SIZE); x < h.x; x += TILE_SIZE) {
			sf::Vertex line[] = {sf::Vertex(sf::Vector2f(x, l.y), sf::Color(0, 0, 255, 50)),
			    sf::Vertex(sf::Vector2f(x, h.y), sf::Color(0, 0, 255, 50))};
			context().systemRef<Render>().target().draw(line, 2, sf::Lines);
		}
		for (auto y = TILE_SIZE * int(l.y / TILE_SIZE); y < h.y; y += TILE_SIZE) {
			sf::Vertex line[] = {sf::Vertex(sf::Vector2f(l.x, y), sf::Color(0, 0, 255, 50)),
			    sf::Vertex(sf::Vector2f(h.x, y), sf::Color(0, 0, 255, 50))};
			context().systemRef<Render>().target().draw(line, 2, sf::Lines);
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		_view.move(-10, 0);
	}

	if (Keyboard::isKeyPressed(Keyboard::D)) {
		_view.move(10, 0);
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		_view.move(0, 10);
	}

	if (Keyboard::isKeyPressed(Keyboard::W)) {
		_view.move(0, -10);
	}

	_world.draw(context().systemRef<Render>().target(), _view);
}
