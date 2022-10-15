#pragma once

#include "alch/common/vector2.hpp"

#include <SDL_events.h>

// #include <SFML/Window/Event.hpp>

struct WindowEvent {
	SDL_Event event;
};

struct WindowUIEvent {
	bool& eventCaptured;
	WindowEvent general;
};

struct WindowResize {
	Vector2i oldSize;
	Vector2i newSize;
};
