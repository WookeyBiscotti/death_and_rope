#include "sprite_component.hpp"

#include <common/consts.hpp>
#include <engine/entity.hpp>
#include <systems/transform/events.hpp>
#include <systems/transform/transform.hpp>
//
#include <SFML/Graphics.hpp>
//

SpriteComponent::SpriteComponent(Entity& entity): Drawable(entity) {
	// entity.
}
// explicit SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex):
//     SpriteComponent(entity), _texture(tex), _sprite(tex->sf()) {
// }
// explicit SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex, const Recti& rect):
//     SpriteComponent(entity), _texture(tex), _sprite(tex->sf(), rect) {
// }

void SpriteComponent::draw(RenderTarget& target, const RenderStates& states) {
	auto tr = entity().get<Transform>();
	if (tr) {
		_sprite.setPosition(tr->p());
		_sprite.setRotation(180 * tr->r() / kPIf);
		target.draw(_sprite, states);
	}
}

// void SpriteComponent::setSprite(const std::shared_ptr<Sprite>& sprite) {
// 	_sprite = sprite;
// }