#include "body.hpp"
#include "collider.hpp"
#include "physics.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/broker/broker.hpp>
#include <systems/position/events.hpp>
#include <systems/position/position.hpp>

Physics::Physics(Context& context): Receiver(context.systemRef<Broker>()), _contex(context) {
	subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart&) {
		// TODO: use config
		update(1);
	});
}

void Physics::add(Collider* collider) {
	const auto& lower = collider->entity().ref<Position>().get() + collider->origin();
	const auto& upper = lower + collider->size();
	collider->_colliderIdx = _colliders.emplace(AABBTree::AABB_t({lower.x, lower.y}, {upper.x, upper.y}), collider);
}

void Physics::remove(Collider* collider) {
	if (collider->_colliderIdx != biss::nullindex) {
		_colliders.remove(collider->_colliderIdx);
	}
}

void Physics::add(Body* body) {
	if (body->type() == Body::STATIC) {
		_static.insert(body);
		body->entity().subscribe<PositionUpdate>(&body->entity(), [this, body](const PositionUpdate& p) {
			auto collider = body->entity().get<Collider>();
			if (!collider) {
				return;
			}
			const auto& lower = p.neW + collider->origin();
			const auto& upper = lower + collider->size();
			_colliders.update(collider->_colliderIdx, AABBTree::AABB_t({lower.x, lower.y}, {upper.x, upper.y}));
		});
	} else {
		_dynamic.insert(body);
	}
}

void Physics::remove(Body* body) {
	if (body->type() == Body::STATIC) {
		body->entity().unsubscribe<PositionUpdate>(&body->entity());
		_static.erase(body);
	} else {
		_dynamic.erase(body);
	}
}

void Physics::update(float dt) {
	for (auto& b : _dynamic) {
		auto& pos = b->entity().ref<Position>();
		auto newPos = pos.get() + b->velocity() * dt;
		pos.set(newPos);
	}

	for (auto& b : _dynamic) {
		if (auto c = b->entity().get<Collider>(); c) {
			const auto& lower = c->entity().ref<Position>().get() + c->origin();
			const auto& upper = lower + c->size();

			AABBTree::AABB_t newAABB({lower.x, lower.y}, {upper.x, upper.y});

			_colliders.query(newAABB, [this, &b, c](const AABBTree::Iterator& it) {
				auto other = *it;
				if (c == other) {
					return true;
				}

				if (auto ob = other->entity().get<Body>(); ob) {
					if (ob->type() == Body::STATIC) {
						b->velocity(-b->velocity());
					}
				}

				return true;
			});

			_colliders.update(c->_colliderIdx, newAABB);
		}
	}
}