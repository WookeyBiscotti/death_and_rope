#include "sprite_component.hpp"

#include <entity.hpp>
#include <logging.hpp>

#include <SFML/Graphics.hpp>
#include <systems/position/position.hpp>

void SpriteComponent::draw(RenderTarget& target, const RenderStates& states)
{
    auto position = entity().get<Position>();
    if (_sprite && position) {
        _sprite->sf().setPosition(position->position());
        target.draw(_sprite->sf(), states);
    }
}