#pragma once

#include <memory>

class Context;

namespace sf {
class RenderWindow;
}

using RenderWindow = sf::RenderWindow;

class Window {
  public:
	Window(Context& context);
	~Window();

	RenderWindow& window();

  private:
	Context& _context;

	std::shared_ptr<RenderWindow> _window;
};