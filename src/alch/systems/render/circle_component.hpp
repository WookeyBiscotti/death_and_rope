#pragma once

#include "alch/common/macros.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/systems/assets/texture.hpp"
#include "alch/systems/broker/receiver.hpp"
#include "drawable.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
//
#include <memory>

class CircleShape: public Drawable, public Receiver {
  public:
	explicit CircleShape(Entity& entity);
	CircleShape(Entity& entity, float r, sf::Color color);

	void draw(RenderTarget& target, const RenderStates& state) override;

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

	ALCH_COMPONENT_NAME(CircleShape);

  private:
	sf::CircleShape _shape;
};
