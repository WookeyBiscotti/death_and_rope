#include "window.hpp"

#include "SFML/Config.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/engine.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/config/config.hpp"
#include "events.hpp"
//
// #include <SFML/Graphics.hpp>
#include <SDL.h>
#include <SDL_video.h>

WindowSystem::WindowSystem(Context& context): Sender(context.systemRef<Broker>()), _context(context) {
	// TODO: check this
	SDL_Init(SDL_INIT_EVERYTHING);

	auto& config = context.systemRef<Config>().staticConfig();
	auto& engineConfig = context.engine.config();
	Uint32 flags{};
	if (config.window.borderless) {
		// flags |= sf::Style::Close | sf::Style::Titlebar;
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if (config.window.fullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	// TODO: check this
	_window = SDL_CreateWindow(config.appName.c_str(), config.window.position.x, config.window.position.y,
	    config.window.size.x, config.window.size.y, flags);

	// _window = std::make_shared<RenderWindow>(
	//     sf::VideoMode(config.window.size.x, config.window.size.y), engineConfig.windowName, flags);
	// _window->setVerticalSyncEnabled(config.window.verticalSync);
	// _window->setPosition(Vector2i{config.window.position});
}

WindowSystem::~WindowSystem() {
	SDL_DestroyWindow(_window);
	SDL_Quit();
};

RenderWindow& WindowSystem::window() {
	return *_window;
}

void WindowSystem::pullEvents() {
	bool captured = false;
	WindowUIEvent e{captured};
	while (SDL_PollEvent(&e.general.event)) {
		if (e.general.event.type != SDL_WINDOWEVENT_CLOSE && e.general.event.type != SDL_WINDOWEVENT_FOCUS_LOST &&
		    e.general.event.type != SDL_WINDOWEVENT_FOCUS_GAINED && e.general.event.type != SDL_WINDOWEVENT_RESIZED) {
			send(e);
		}
		if (!captured) {
			send(e.general);
		}
	}
}
