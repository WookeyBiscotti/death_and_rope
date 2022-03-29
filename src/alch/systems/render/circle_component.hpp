#pragma once

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "drawable.hpp"

#include <common/macros.hpp>
#include <common/rect.hpp>
#include "alch/common/vector2.hpp"
#include <systems/assets/texture.hpp>
//
#include <SFML/Graphics/CircleShape.hpp>
//
#include <memory>

class CircleComponent: public Drawable {
  public:
	explicit CircleComponent(Entity& entity);
	CircleComponent(Entity& entity, float r, sf::Color color);

	void draw(RenderTarget& target, const RenderStates& state) override;

	template<class Archive>
	void serialize(Archive& ar);

  private:
	sf::CircleShape _shape;
};
