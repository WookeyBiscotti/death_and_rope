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

void RectShape::save(OArchive& archive) const {
	archive(_shape.getSize());
	archive(_shape.getFillColor());
}

void RectShape::load(IArchive& archive) {
	Vector2f size;
	Color c;
	archive(size);
	archive(c);
	_shape.setSize(size);
	_shape.setFillColor(c);
	_shape.setOrigin(size.x / 2, size.y / 2);
}
