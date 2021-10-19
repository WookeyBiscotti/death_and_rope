#pragma once

#include <scene.hpp>

#include <SFML/Graphics/View.hpp>

class WorldEditor : public Scene
{
public:
    WorldEditor(Context& context);

    void onFrame() override;

    bool active() const override { return true; }
    void active(bool active) override {}

private:
    sf::View _view;
};