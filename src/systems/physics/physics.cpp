#include "physics.hpp"

#include "body.hpp"
#include "collider.hpp"

#include <common/rect.hpp>
#include <common/vector2.hpp>
#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/broker/broker.hpp>
#include <systems/position/events.hpp>
#include <systems/position/position.hpp>

Physics::Physics(Context& context): Receiver(context.systemRef<Broker>()), _contex(context), _world({0, 100}) {
	subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart&) {
		// TODO: use config
		update(1 / 60.f);
	});
}

void Physics::update(float dt) {
	// TODO: get from config
	_world.Step(dt, 4, 4);

	auto body = _world.GetBodyList();

	while (body) {
		if (body->GetType() != b2_staticBody && body->IsAwake()) {
			body->GetUserData().entity->ref<Position>().set(from(body->GetPosition()));
		}
		body = body->GetNext();
	}
}