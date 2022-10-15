#pragma once

#include <SFML/Window/Event.hpp>

namespace al {

struct UIHovered {
	const sf::Event::MouseMoveEvent& event;
};

struct UIMouseMove {
	const sf::Event::MouseMoveEvent& event;
};

struct UIUnhovered {
	const sf::Event::MouseMoveEvent& event;
};

struct UIMouseButtonPressed {
	const sf::Event::MouseButtonEvent& event;
};

struct UIMouseButtonReleased {
	const sf::Event::MouseButtonEvent& event;
};

struct UIMouseDragStart {
	const sf::Event::MouseButtonEvent& event;
};

struct UIMouseDrag {
	const sf::Event::MouseMoveEvent& event;
};

struct UIMouseDragStop {
	const sf::Event::MouseButtonEvent& event;
};

struct UIMouseWheel {
	const sf::Event::MouseWheelScrollEvent& event;
};

}
