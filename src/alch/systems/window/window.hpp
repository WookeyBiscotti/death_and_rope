#pragma once

#include "alch/common/optional.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/config/config_handler.hpp"

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

	std::vector<Vector2<int>> resolutions() const;

	void pullEvents();

  private:
	void updateWindowFromConfig();

  private:
	SharedPtr<RenderWindow> _window;
	ConfigHandler _config;
};

} // namespace al
