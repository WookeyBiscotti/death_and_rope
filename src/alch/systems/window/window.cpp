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
		flags |= sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize;
	}
	if (config.window.fullscreen) {
		flags |= sf::Style::Fullscreen;
	}
	_window = SharedPtr<RenderWindow>::make(sf::VideoMode(config.window.size.x, config.window.size.y),
	    engineConfig.windowName, flags);
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
	//TODO: rewrite this
	static sf::Vector2u p;
	while (_window->pollEvent(e.general.event) ) {
		if (e.general.event.type == sf::Event::Resized && p != Vector2u{e.general.event.size.width, e.general.event.size.height}) {
			p = Vector2u{e.general.event.size.width, e.general.event.size.height};
			auto& config = _context.systemRef<Config>().staticConfig();
			auto& engineConfig = _context.systemRef<Engine>().config();
			sf::Uint32 flags{};
			if (!config.window.borderless) {
				flags |= sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize;
			}
			if (config.window.fullscreen) {
				flags |= sf::Style::Fullscreen;
			}
			auto pos = _window->getPosition();
			_window->create(
			    sf::VideoMode(e.general.event.size.width, e.general.event.size.height), engineConfig.windowName, flags);
				_window->setPosition(pos);
		}
		send(e);
		// if (e.general.event.type != sf::Event::Closed && e.general.event.type != sf::Event::LostFocus &&
		// e.general.event.type != sf::Event::GainedFocus && e.general.event.type != sf::Event::Resized) {
		// }
		// if (!captured) {
		// 	send(e.general);
		// }
	}
}
