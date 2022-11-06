#pragma once

#include "alch/engine/system.hpp"

//
#include <memory>

namespace sf {
class RenderWindow;
}

namespace al {

class Context;

using RenderWindow = sf::RenderWindow;

class Window: public System {
  public:
	explicit Window(Context& context);
	~Window();

	RenderWindow& window();

	void pullEvents();

  private:
	std::shared_ptr<RenderWindow> _window;
};

} // namespace al
