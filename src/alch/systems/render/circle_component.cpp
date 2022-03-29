#include "circle_component.hpp"

#include "alch/common/color.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/render/drawable.hpp"
#include "alch/systems/transform/events.hpp"
#include "alch/systems/transform/transform.hpp"

#include <SFML/Graphics.hpp>

CircleComponent::CircleComponent(Entity& entity): Drawable(entity) {
	entity.subscribe<PositionUpdate>(&entity, this, [this](const PositionUpdate& p) { _shape.setPosition(p.neW); });
};

CircleComponent::CircleComponent(Entity& entity, float r, sf::Color color): CircleComponent(entity) {
	_shape.setRadius(r);
	_shape.setFillColor(color);
};

void CircleComponent::draw(RenderTarget& target, const RenderStates& state) {
	target.draw(_shape, state);
}

void CircleComponent::serialize(OArchive& ar) const {
	ar(_shape.getRadius());
	::serialize(ar, _shape.getFillColor());
}

void CircleComponent::deserialize(IArchive& ar) {
	float r;
	Color c;
	ar(r);
	::serialize(ar, c);
	_shape.setRadius(r);
	_shape.setFillColor(c);
}
