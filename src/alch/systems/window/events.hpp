#pragma once

#include "alch/common/vector2.hpp"

#include <SFML/Window/Event.hpp>

namespace al {

struct WindowEvent {
	sf::Event event;
};

struct WindowUIEvent {
	bool& eventCaptured;
	WindowEvent general;
};

struct WindowResize {
	Vector2i oldSize;
	Vector2i newSize;
};

}
