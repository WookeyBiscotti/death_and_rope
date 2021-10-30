#include "window.hpp"

#include "events.hpp"

#include <context.hpp>
#include <systems/broker/broker.hpp>
//
#include <SFML/Graphics.hpp>

Window::Window(Context& context): Sender(context.systemRef<Broker>()), _context(context) {
	_window = std::make_shared<RenderWindow>(sf::VideoMode(640, 480), "Death and rope");
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