#pragma once

#include "alch/engine/system.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/optional.hpp"
#include "alch/common/vector2.hpp"

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
	SharedPtr<RenderWindow> _window;
};

} // namespace al
