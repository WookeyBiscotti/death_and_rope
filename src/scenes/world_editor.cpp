#include "world_editor.hpp"

#include <SFML/Graphics.hpp>
#include <asset_cache.hpp>
#include <config.hpp>
#include <context.hpp>
#include <imgui_utils.hpp>
#include <tile_utils.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

WorldEditor::WorldEditor(Context& context)
    : Scene(context)
{
    _view = context.target.getView();
}

void WorldEditor::onEvent(const sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed) {
        const auto p = context().target.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });
        paintPoint(p);
    }
}

void WorldEditor::paintPoint(const Vector2f& p)
{
    auto& tile = _world.tileGetOrCreate(p);
    tile.typeId = _currentTileType;
}

void WorldEditor::onFrame()
{
    if (_showSaveDialog) {
        ImGui::InputText("World name", &_saveName);
        if (ImGui::Button("Ok")) {
            _showSaveDialog = false;
            context().cache.world(_world, _saveName);
        }
        if (ImGui::Button("Cancel")) {
            _showSaveDialog = false;
        }
        return;
    }

    ImGui::Begin("World editor");
    if (ImGui::Button("Save")) {
        _showSaveDialog = true;
    }
    if (ImGui::Button("Back")) {
        context().nextScene = context().cache.scene("dev_menu");
    }
    ImGui::End();

    ImGui::Begin("Paint tool");
    ImGui::InputInt("Type", &_currentTileType);
    if (_currentTileType < 0) {
        _currentTileType = 0;
    }
    ImGui::End();

    context().target.setView(_view);
    {
        sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(-1024, 0), sf::Color(0, 0, 255, 100)),
                               sf::Vertex(sf::Vector2f(1024, 0), sf::Color(0, 0, 255, 100)) };
        sf::Vertex line2[] = { sf::Vertex(sf::Vector2f(0, -1024), sf::Color(0, 0, 255, 100)),
                               sf::Vertex(sf::Vector2f(0, 1024), sf::Color(0, 0, 255, 100)) };

        context().target.draw(line1, 2, sf::Lines);
        context().target.draw(line2, 2, sf::Lines);
    }
    {
        const auto l = _view.getCenter() - 0.5f * _view.getSize();
        const auto h = _view.getCenter() + 0.5f * _view.getSize();
        for (auto x = TILE_SIZE * int(l.x / TILE_SIZE); x < h.x; x += TILE_SIZE) {
            sf::Vertex line[] = { sf::Vertex(sf::Vector2f(x, l.y), sf::Color(0, 0, 255, 50)),
                                  sf::Vertex(sf::Vector2f(x, h.y), sf::Color(0, 0, 255, 50)) };
            context().target.draw(line, 2, sf::Lines);
        }
        for (auto y = TILE_SIZE * int(l.y / TILE_SIZE); y < h.y; y += TILE_SIZE) {
            sf::Vertex line[] = { sf::Vertex(sf::Vector2f(l.x, y), sf::Color(0, 0, 255, 50)),
                                  sf::Vertex(sf::Vector2f(h.x, y), sf::Color(0, 0, 255, 50)) };
            context().target.draw(line, 2, sf::Lines);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _view.move(-10, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _view.move(10, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _view.move(0, 10);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _view.move(0, -10);
    }

    _world.draw(context().target, _view);
}
