#include "window.hpp"

#include <SFML/Graphics.hpp>

Window::Window(Context& context): _context(context) {
	_window = std::make_shared<RenderWindow>(sf::VideoMode(640, 480), "Death and rope");
}

Window::~Window(){};

RenderWindow& Window::window() {
	return *_window;
}