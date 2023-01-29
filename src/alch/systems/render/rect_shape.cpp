#include "rect_shape.hpp"

#include "alch/engine/entity.hpp"
#include "alch/systems/render/drawable.hpp"
#include "alch/systems/transform/events.hpp"
#include "alch/systems/transform/transform.hpp"

#include <SFML/Graphics.hpp>
#include <cereal/cereal.hpp>

using namespace al;

RectShape::RectShape(Entity& entity): Drawable(entity), Transmitter(entity.context().systemRef<Broker>()) {
	subscribe<PositionUpdate>(&entity, [this](const PositionUpdate& p) { _shape.setPosition(p.neW); });
	_shape.setPosition(entity.tr().p());
}

RectShape::RectShape(Entity& entity, Vector2f size, sf::Color color): RectShape(entity) {
	_shape.setSize(size);
	_shape.setFillColor(color);
	_shape.setOrigin(size.x / 2, size.y / 2);
}

void RectShape::draw(RenderTarget& target, const RenderStates& state) {
	target.draw(_shape, state);
}

void RectShape::save(VarOArchive& ar) const {
	al::save(ar, _shape.getSize());
	al::save(ar, _shape.getFillColor());
}

void RectShape::load(VarIArchive& ar) {
	Vector2f size;
	Color c;
	al::load(ar, size);
	al::load(ar, c);
	_shape.setSize(size);
	_shape.setFillColor(c);
	_shape.setOrigin(size.x / 2, size.y / 2);
}
