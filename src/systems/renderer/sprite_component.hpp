#pragma once

#include "drawable.hpp"

#include <memory>
#include <sprite.hpp>
#include <vector2.hpp>

class SpriteComponent : public Drawable
{
public:
    SpriteComponent(Entity& entity)
        : Drawable(entity)
    {}

    void draw(RenderTarget& target, const RenderStates& state) override;

private:
    std::shared_ptr<Sprite> _sprite;
};