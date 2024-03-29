#include "circle_component.hpp"

#include "alch/common/color.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/render/drawable.hpp"
#include "alch/systems/transform/events.hpp"
#include "alch/systems/transform/transform.hpp"

#include <SFML/Graphics.hpp>

using namespace al;

CircleShape::CircleShape(Entity& entity): Drawable(entity), Transmitter(entity.context().systemRef<Broker>()) {
	subscribe<PositionUpdate>(&entity, [this](const PositionUpdate& p) { _shape.setPosition(p.neW); });
	_shape.setPosition(entity.tr().p());
}

CircleShape::CircleShape(Entity& entity, float r, sf::Color color): CircleShape(entity) {
	_shape.setRadius(r);
	_shape.setFillColor(color);
	_shape.setOrigin(r, r);
}

void CircleShape::draw(RenderTarget& target, const RenderStates& state) {
	target.draw(_shape, state);
}

void CircleShape::save(OArchive& archive) const {
	archive(_shape.getRadius());
	archive(_shape.getFillColor());
}

void CircleShape::load(IArchive& archive) {
	float r;
	Color c;
	archive(r);
	archive(c);
	_shape.setRadius(r);
	_shape.setFillColor(c);
	_shape.setOrigin(r, r);
}
