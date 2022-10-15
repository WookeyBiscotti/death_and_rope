#pragma once

#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"
#include "alch/systems/render/render_state.hpp"

// namespace sf {
// class RenderTarget;
// class RenderStates;
// } // namespace sf

// using RenderTarget = sf::RenderTarget;
// using RenderStates = sf::RenderStates;

// using RenderTarget = sf::RenderTarget;
// using RenderStates = sf::RenderStates;

namespace alch {

class Drawable: public Component {
  public:
	Drawable(Entity& entity);
	virtual ~Drawable();

	virtual void draw(const RenderState& state) = 0;
};

} // namespace alch
