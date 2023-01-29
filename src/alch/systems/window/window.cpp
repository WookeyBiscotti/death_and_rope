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

Window::Window(Context& context): System(context), _config(context.systemRef<ConfigSystem>()) {
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

	const auto pos = _window->getPosition();

	if (auto winSize = config.value<Vector2f>(WINDOW_SIZE)) {
		_window->create(sf::VideoMode(winSize->y, winSize->x),
		    config.valueOr<String>(APPLICATION_NAME, ""), flags);
	} else {
		if (config.valueOr(WINDOW_FULLSCREEN, false)) {
			_window->create(sf::VideoMode::getDesktopMode(), config.valueOr<String>(APPLICATION_NAME, ""), flags);
		} else {
			_window->create(sf::VideoMode(800, 600), config.valueOr<String>(APPLICATION_NAME, ""), flags);
		}
	}
	if (auto size = config.value<Vector2f>(WINDOW_POSITION)) {
		_window->setPosition(Vector2i(size->x, size->y));
	} else {
		_window->setPosition(pos);
	}
	_config.subscribe(config::WINDOW_POSITION, [this](const String&) { updateWindowFromConfig(); });
	_config.subscribe(config::WINDOW_SIZE, [this](const String&) { updateWindowFromConfig(); });
	_config.subscribe(config::WINDOW_FULLSCREEN, [this](const String&) { updateWindowFromConfig(); });
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

std::vector<Vector2<int>> Window::resolutions() const {
	std::vector<Vector2<int>> result;
	for (const auto& vm : sf::VideoMode::getFullscreenModes()) {
		if (vm.bitsPerPixel == 32) {
			result.push_back(Vector2<int>(vm.width, vm.height));
		}
	}

	return result;
}
