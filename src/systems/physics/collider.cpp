#include "collider.hpp"

#include "physics.hpp"
//
#include <engine/context.hpp>
#include <engine/entity.hpp>
//
#include <box2d/box2d.h>

Collider::~Collider() {
	if (_fixture) {
		auto& w = entity().context().systemRef<Physics>()._world;
		entity().ref<Body>()._body->DestroyFixture(_fixture);
	}
}

Collider::Collider(Entity& entity, Box_t, const Vector2f& size, float density, const Vector2f& origin):
    Component(entity) {
	b2PolygonShape s;
	s.SetAsBox(size.x * 0.5f, size.y * 0.5f, {size.x * 0.5f, size.y * 0.5f}, {});
	entity.ref<Body>()._body->CreateFixture(&s, density);
}