#include "sprite_component.hpp"

#include "alch/common/consts.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/transform/events.hpp"
#include "alch/systems/transform/transform.hpp"
//
#include <SFML/Graphics.hpp>
//

using namespace al;

Sprite::Sprite(Entity& entity): Drawable(entity), Transmitter(entity.context().systemRef<Broker>()) {
	subscribe<PositionUpdate>(&entity, [this](const PositionUpdate& p) { _sprite.setPosition(p.neW); });
	subscribe<RotationUpdate>(
	    &entity, [this](const RotationUpdate& r) { _sprite.setRotation(180 * r.neW / kPIf); });
}

Sprite::Sprite(Entity& entity, const std::shared_ptr<Texture>& tex): Sprite(entity) {
	_texture = tex;
	_sprite.setTexture(tex->sf());
}

Sprite::Sprite(Entity& entity, const std::shared_ptr<Texture>& tex, const Recti& rect): Sprite(entity) {
	_texture = tex;
	_sprite.setTexture(tex->sf());
	_sprite.setTextureRect(rect);
}

void Sprite::draw(RenderTarget& target, const RenderStates& states) {
	target.draw(_sprite, states);
}

void Sprite::serialize(OArchive& ar) const {
	ar(!!_texture);
	if (_texture) {
		ar(_texture->name());
	}
	::serialize(ar, _sprite.getTextureRect());
	::serialize(ar, _sprite.getPosition());
	ar(_sprite.getRotation());
}

void Sprite::deserialize(IArchive& ar) {
	bool inited;
	ar(inited);
	if (inited) {
		std::string path;
		ar(path);
		_texture = entity().context().systemRef<AssetCache>().texture(path);
		_sprite.setTexture(_texture->sf());
	}

	std::decay_t<decltype(_sprite.getTextureRect())> rect;
	::serialize(ar, rect);
	_sprite.setTextureRect(rect);

	std::decay_t<decltype(_sprite.getPosition())> position;
	::serialize(ar, position);
	_sprite.setPosition(position);

	std::decay_t<decltype(_sprite.getRotation())> rotation;
	ar(rotation);
	_sprite.setRotation(rotation);
}
