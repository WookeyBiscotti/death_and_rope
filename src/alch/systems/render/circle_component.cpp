#include "circle_component.hpp"

#include "engine/entity.hpp"
#include "systems/render/drawable.hpp"
#include "systems/transform/events.hpp"
#include "systems/transform/transform.hpp"

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

template<class Archive>
void CircleComponent::serialize(Archive& ar){
    
}
