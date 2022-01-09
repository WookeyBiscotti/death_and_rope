#include "sprite_component.hpp"

#include <common/consts.hpp>
#include <engine/entity.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/transform/events.hpp>
#include <systems/transform/transform.hpp>
//
#include <SFML/Graphics.hpp>
//

SpriteComponent::SpriteComponent(Entity& entity): Drawable(entity) {
	entity.subscribe<PositionUpdate>(&entity, this, [this](const PositionUpdate& p) { _sprite.setPosition(p.neW); });
	entity.subscribe<RotationUpdate>(&entity, this,
	    [this](const RotationUpdate& r) { _sprite.setRotation(180 * r.neW / kPIf); });
}

SpriteComponent::SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex): SpriteComponent(entity) {
	_texture = tex;
	_sprite.setTexture(tex->sf());
}

SpriteComponent::SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex, const Recti& rect):
    SpriteComponent(entity) {
	_texture = tex;
	_sprite.setTexture(tex->sf());
	_sprite.setTextureRect(rect);
}

void SpriteComponent::draw(RenderTarget& target, const RenderStates& states) {
	target.draw(_sprite, states);
}

template<>
void SpriteComponent::serialize(OArchive& ar) {
	ar(!!_texture);
	if (_texture) {
		ar(_texture->name());
	}
	::serialize(ar, _sprite.getPosition());
	ar(_sprite.getRotation());
}

template<>
void SpriteComponent::serialize(IArchive& ar) {
	bool inited;
	ar(inited);
	if (inited) {
		std::string path;
		ar(path);
		_texture = entity().context().systemRef<AssetCache>().texture(path);
	}

	std::decay_t<decltype(_sprite.getPosition())> position;
	::serialize(ar, position);
	_sprite.setPosition(position);

	std::decay_t<decltype(_sprite.getRotation())> rotation;
	ar(rotation);
	_sprite.setRotation(rotation);
}