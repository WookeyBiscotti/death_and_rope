#include "sprite_component.hpp"

#include <common/consts.hpp>
#include <engine/entity.hpp>
#include <systems/transform/transform.hpp>
//
#include <SFML/Graphics.hpp>
//

void SpriteComponent::draw(RenderTarget& target, const RenderStates& states) {
	auto tr = entity().get<Transform>();
	if (_sprite && tr) {
		_sprite->sf().setPosition(tr->p());
		_sprite->sf().setRotation(180 * tr->r() / kPIf);
		target.draw(_sprite->sf(), states);
	}
}

void SpriteComponent::setSprite(const std::shared_ptr<Sprite>& sprite) {
	_sprite = sprite;
}