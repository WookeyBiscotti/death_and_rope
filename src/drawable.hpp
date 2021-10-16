#pragma once

#include <SFML/Graphics.hpp>

class Drawable
{
public:
    virtual ~Drawable() = default;

    virtual void draw(sf::RenderTarget& target) = 0;
};