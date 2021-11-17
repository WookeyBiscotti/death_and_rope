#pragma once

#include <systems/broker/sender.hpp>
//
#include <memory>

class Context;

namespace sf {
class RenderWindow;
}

using RenderWindow = sf::RenderWindow;

class Window: private Sender {
  public:
	explicit Window(Context& context);
	~Window();

	RenderWindow& window();

	void pullEvents();

  private:
	Context& _context;

	std::shared_ptr<RenderWindow> _window;
};