#include "physics.hpp"

#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/transform/events.hpp"
#include "alch/systems/transform/transform.hpp"
#include "body.hpp"
#include "collider.hpp"
#include "events.hpp"

#include <box2d/b2_world_callbacks.h>
#include <memory>

using namespace al;

class al::ContactListener: public b2ContactListener {
	Physics& _physics;

  public:
	ContactListener(Physics& physics): _physics(physics){};

	std::vector<PhysicsBeginContact> beginContacts;

	void BeginContact(b2Contact* contact) {
		auto& ba = *contact->GetFixtureA()->GetBody();
		auto& bb = *contact->GetFixtureB()->GetBody();
		auto& ea = *ba.GetUserData().entity;
		auto& eb = *bb.GetUserData().entity;
		PhysicsBeginContact bc{ea, ba, eb, bb};
		beginContacts.push_back(bc);
	}

	// void EndContact(b2Contact* contact) { /* handle end event */
	// }

	// void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) { /* handle pre-solve event */
	// }

	// void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) { /* handle post-solve event */
	// }
};

Physics::Physics(Context& context): Receiver(context.systemRef<Broker>()), _contex(context), _world({0, 0}) {
	_contactListner = std::make_unique<ContactListener>(*this);
	_world.SetContactListener(_contactListner.get());
	subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart&) {
		// TODO: use config
		update(1 / 60.f);
	});
}

Physics::~Physics(){};

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

	for (const auto& bc : _contactListner->beginContacts) {
		bc.ea.send<PhysicsBeginContact>(bc);
		bc.eb.send<PhysicsBeginContact>(bc);
	}
	_contactListner->beginContacts.clear();
}

void Physics::castRay(Vector2f start, Vector2f end, const std::function<bool(const CastRayResult& result)>& callback) {
	class RayCastCallback: public b2RayCastCallback {
	  public:
		CastRayResult result{};
		Physics& physics;
		decltype(callback)& callback;
		RayCastCallback(Physics& physics, decltype(callback) callback): physics(physics), callback(callback){};
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
			result.entity = fixture->GetBody()->GetUserData().entity;
			return callback(result);
		};
	} rayCallback(*this, callback);

	_world.RayCast(&rayCallback, to(start), to(end));
}
