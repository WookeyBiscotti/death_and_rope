#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "alch/common/macros.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/systems/assets/texture.hpp"
#include "drawable.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
//
#include <memory>

class RectShape: public Drawable {
  public:
	explicit RectShape(Entity& entity);
	RectShape(Entity& entity, Vector2f size, sf::Color color);

	void draw(RenderTarget& target, const RenderStates& state) override;

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

	ALCH_COMPONENT_NAME(RectShape);

  private:
	sf::RectangleShape _shape;
};
