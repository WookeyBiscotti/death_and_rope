#include "collider.hpp"
#include "physics.hpp"
//
#include <engine/context.hpp>
#include <engine/entity.hpp>

Collider::Collider(Entity& entity, const Vector2f& size, const Vector2f& origin):
    Component(entity), _size(size), _origin(origin) {
	entity.context().systemRef<Physics>().add(this);
}

Collider::~Collider() {
	entity().context().systemRef<Physics>().remove(this);
}