#pragma once

#include "alch/common/macros.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/systems/assets/texture.hpp"
#include "drawable.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
//
#include <memory>

class CircleComponent: public Drawable {
  public:
	explicit CircleComponent(Entity& entity);
	CircleComponent(Entity& entity, float r, sf::Color color);

	void draw(RenderTarget& target, const RenderStates& state) override;

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

  private:
	sf::CircleShape _shape;
};
