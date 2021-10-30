#include "sprite_component.hpp"

#include <entity.hpp>

#include <SFML/Graphics.hpp>
#include <systems/position/position.hpp>

void SpriteComponent::draw(RenderTarget& target, const RenderStates& states)
{
    auto position = entity().get<Position>();
    if (_sprite && position) {
        _sprite->sf().setPosition(position->get());
        target.draw(_sprite->sf(), states);
    }
}

void SpriteComponent::setSprite(const std::shared_ptr<Sprite>& sprite)
{
    _sprite = sprite;
}