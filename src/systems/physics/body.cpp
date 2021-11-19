#include "body.hpp"
#include "physics.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>

Body::Body(Entity& entity, Type type, float mass): Component(entity), _type(type), _mass(mass) {
	entity.context().systemRef<Physics>().add(this);
}

Body::~Body() {
	entity().context().systemRef<Physics>().remove(this);
}

void Body::type(Type type) {
	if (_type != type) {
		entity().context().systemRef<Physics>().remove(this);
		_type = type;
		entity().context().systemRef<Physics>().add(this);
	}
}