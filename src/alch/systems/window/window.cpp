#include "window.hpp"

#include "SFML/Config.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/engine.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/config/config.hpp"
#include "events.hpp"
//
#include <SFML/Graphics.hpp>

using namespace al;

Window::Window(Context& context): System(context) {
	auto& config = context.systemRef<Config>().staticConfig();
	auto& engineConfig = context.systemRef<Engine>().config();
	sf::Uint32 flags{};
	if (!config.window.borderless) {
		flags |= sf::Style::Close | sf::Style::Titlebar;
	}
	if (config.window.fullscreen) {
		flags |= sf::Style::Fullscreen;
	}
	_window = std::make_shared<RenderWindow>(
	    sf::VideoMode(config.window.size.x, config.window.size.y), engineConfig.windowName, flags);
	_window->setVerticalSyncEnabled(config.window.verticalSync);
	_window->setPosition(Vector2i{config.window.position});
}

Window::~Window(){};

RenderWindow& Window::window() {
	return *_window;
}

void Window::pullEvents() {
	bool captured = false;
	WindowUIEvent e{captured};
	while (_window->pollEvent(e.general.event)) {
		if (e.general.event.type != sf::Event::Closed && e.general.event.type != sf::Event::LostFocus &&
		    e.general.event.type != sf::Event::GainedFocus && e.general.event.type != sf::Event::Resized) {
			send(e);
		}
		if (!captured) {
			send(e.general);
		}
	}
}
