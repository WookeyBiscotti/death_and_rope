#pragma once

#include <systems/broker/sender.hpp>
//
#include <memory>

class Context;

namespace sf {
class RenderWindow;
}

using RenderWindow = sf::RenderWindow;

class Window: public Sender {
  public:
	Window(Context& context);
	~Window();

	RenderWindow& window();

	void pullEvents();

  private:
	Context& _context;

	std::shared_ptr<RenderWindow> _window;
};