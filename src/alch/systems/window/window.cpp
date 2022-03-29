#include "SFML/Config.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include "events.hpp"
#include "window.hpp"

#include <engine/context.hpp>
#include <engine/engine.hpp>
#include <systems/broker/broker.hpp>
#include <systems/config/config.hpp>
//
#include <SFML/Graphics.hpp>

Window::Window(Context& context): Sender(context.systemRef<Broker>()), _context(context) {
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
	WindowEvent event;
	while (_window->pollEvent(event.event)) {
		send(event);
	}
}