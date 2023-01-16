#include "window.hpp"

#include "SFML/Config.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/engine.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/config/config.hpp"
#include "alch/systems/config/config_defs.hpp"
#include "events.hpp"
//
#include <SFML/Graphics.hpp>

using namespace al;

Window::Window(Context& context): System(context) {
	updateWindowFromConfig();
}

Window::~Window(){};

RenderWindow& Window::window() {
	return *_window;
}

void Window::updateWindowFromConfig() {
	if (!_window) {
		_window = SharedPtr<RenderWindow>::make();
	}

	using namespace al::config;
	auto& config = _context.systemRef<ConfigSystem>();

	sf::Uint32 flags{};
	if (!config.valueOr(WINDOW_BORDERLESS, false)) {
		flags |= sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize;
	}
	if (config.valueOr(WINDOW_FULLSCREEN, false)) {
		flags |= sf::Style::Fullscreen;
	}

	if ((!config.value<int64_t>(WINDOW_SIZE_W) || !config.value<int64_t>(WINDOW_SIZE_H))) {
		if (config.valueOr(WINDOW_FULLSCREEN, false)) {
			_window->create(sf::VideoMode::getDesktopMode(), config.valueOr<String>(APPLICATION_NAME, ""), flags);
		} else {
			_window->create(sf::VideoMode(800, 600), config.valueOr<String>(APPLICATION_NAME, ""), flags);
		}
	} else {
		_window->create(sf::VideoMode(*config.value<int64_t>(WINDOW_SIZE_W), *config.value<int64_t>(WINDOW_SIZE_H)),
		    config.valueOr<String>(APPLICATION_NAME, ""), flags);
	}
	_window->setPosition(
	    Vector2i(config.valueOr<int64_t>(WINDOW_POSITION_X, 0), config.valueOr<int64_t>(WINDOW_POSITION_Y, 0)));
}

void Window::pullEvents() {
	using namespace al::config;
	bool captured = false;
	WindowUIEvent e{captured};
	// TODO: rewrite this
	while (_window->pollEvent(e.general.event)) {
		send(e);
		// if (e.general.event.type != sf::Event::Closed && e.general.event.type != sf::Event::LostFocus &&
		// e.general.event.type != sf::Event::GainedFocus && e.general.event.type != sf::Event::Resized) {
		// }
		if (!captured) {
			send(e.general);
		}
	}
}
