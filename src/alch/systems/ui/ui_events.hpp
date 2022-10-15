#pragma once

// #include <SFML/Window/Event.hpp>
#include <SDL_events.h>

struct UIHovered {
	// const sf::Event::MouseMoveEvent& event;
	const SDL_MouseMotionEvent& event;
};

struct UIMouseMove {
	const SDL_MouseMotionEvent& event;
};

struct UIUnhovered {
	const SDL_MouseMotionEvent& event;
};

struct UIMouseButtonPressed {
	const SDL_MouseButtonEvent& event;
};

struct UIMouseButtonReleased {
	const SDL_MouseButtonEvent& event;
};

struct UIMouseDragStart {
	const SDL_MouseButtonEvent& event;
};

struct UIMouseDrag {
	const SDL_MouseMotionEvent& event;
};

struct UIMouseDragStop {
	const SDL_MouseButtonEvent& event;
};

struct UIMouseWheel {
	const SDL_MouseWheelEvent& event;
};
