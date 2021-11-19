#include "events.hpp"
#include "position.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>

void Position::set(const Vector2f& position) {
	if (_position != position) {
		PositionUpdate e{_position, position};
		entity().send(e);
		_position = position;
	}
}