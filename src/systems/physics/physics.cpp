#include "physics.hpp"

#include "body.hpp"
#include "collider.hpp"

#include <common/rect.hpp>
#include <common/vector2.hpp>
#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/broker/broker.hpp>
#include <systems/transform/events.hpp>
#include <systems/transform/transform.hpp>

Physics::Physics(Context& context): Receiver(context.systemRef<Broker>()), _contex(context), _world({0, 100}) {
	subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart&) {
		// TODO: use config
		update(1 / 60.f);
	});
}

void Physics::update(float dt) {
	// TODO: get from config
	_world.Step(dt, 2, 2);

	auto body = _world.GetBodyList();

	while (body) {
		if (body->GetType() != b2_staticBody && body->IsAwake()) {
			auto& tr = body->GetUserData().entity->ref<Transform>();
			tr.p(from(body->GetPosition()));
			tr.r(body->GetAngle());
		}
		body = body->GetNext();
	}
}