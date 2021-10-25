#pragma once

#include <scene.hpp>
#include <world.hpp>

#include <SFML/Graphics/View.hpp>

class WorldEditor : public Scene
{
public:
    WorldEditor(Context& context);

    void onFrame() override;
    void onEvent(const sf::Event& e) override;

    bool active() const override { return true; }
    void active(bool active) override {}

private:
    void paintPoint(const Vector2f& p);

private:
    bool _showLoadDialog = false;
    std::vector<std::string> _worldsList;

    bool _showSaveDialog = false;
    std::string _saveName;

    int _currentTileType = 0;
    sf::View _view;
    World _world;
};