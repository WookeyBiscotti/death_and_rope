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
	subscribe<RotationUpdate>(&entity, [this](const RotationUpdate& r) { _sprite.setRotation(180 * r.neW / kPIf); });
}

Sprite::Sprite(Entity& entity, const SharedPtr<Texture>& tex): Sprite(entity) {
	_texture = tex;
	_sprite.setTexture(tex->sf());
}

Sprite::Sprite(Entity& entity, const SharedPtr<Texture>& tex, const Recti& rect): Sprite(entity) {
	_texture = tex;
	_sprite.setTexture(tex->sf());
	_sprite.setTextureRect(rect);
}

void Sprite::draw(RenderTarget& target, const RenderStates& states) {
	target.draw(_sprite, states);
}

void Sprite::save(VarOArchive& archive) const {
	al::save(archive, !!_texture);
	if (_texture) {
		al::save(archive, _texture->name());
	}
	al::save(archive, _sprite.getTextureRect());
	al::save(archive, _sprite.getPosition());
	al::save(archive, _sprite.getRotation());
}

void Sprite::load(VarIArchive& archive) {
	bool inited;
	std::decay_t<decltype(_sprite.getTextureRect())> rect;
	std::decay_t<decltype(_sprite.getPosition())> position;
	std::decay_t<decltype(_sprite.getRotation())> rotation;

	al::load(archive, inited);
	if (inited) {
		std::string path;
		al::load(archive, path);
		_texture = entity().context().systemRef<AssetCache>().texture(path);
		_sprite.setTexture(_texture->sf());
	}
	al::load(archive, rect);
	al::load(archive, position);
	al::load(archive, rotation);

	_sprite.setTextureRect(rect);
	_sprite.setPosition(position);
	_sprite.setRotation(rotation);
}
