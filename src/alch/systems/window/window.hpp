#pragma once

#include "alch/engine/system.hpp"
#include "alch/systems/broker/sender.hpp"
//
#include <memory>

class Context;

class SDL_Window;

using RenderWindow = SDL_Window;

class WindowSystem: private Sender, public System {
  public:
	explicit WindowSystem(Context& context);
	~WindowSystem();

	RenderWindow& window();

	void pullEvents();

  private:
	Context& _context;

	SDL_Window* _window;
};
