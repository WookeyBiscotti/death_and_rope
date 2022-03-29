#include "physics.hpp"

#include "body.hpp"
#include "collider.hpp"

#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/transform/events.hpp"
#include "alch/systems/transform/transform.hpp"

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
