#include "events.hpp"
#include "position.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <systems/broker/sender_component.hpp>

void Position::set(const Vector2f& position) {
	if (_position != position) {
		if (auto s = entity().get<SenderComponent>(); s) {
			PositionUpdate e{_position, position};
			s->send(e);
		}
		_position = position;
	}
}