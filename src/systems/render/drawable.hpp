#pragma once

#include <common/vector2.hpp>
#include <engine/component.hpp>

namespace sf {
class RenderTarget;
class RenderStates;
} // namespace sf

using RenderTarget = sf::RenderTarget;
using RenderStates = sf::RenderStates;

class Drawable: public Component {
  public:
	Drawable(Entity& entity): Component(entity) {}

	virtual void draw(RenderTarget& target, const RenderStates& state) = 0;
};