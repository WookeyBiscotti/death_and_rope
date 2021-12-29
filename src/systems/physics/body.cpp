#include "body.hpp"

#include "physics.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <systems/position/position.hpp>
//
#include <box2d/box2d.h>

Body::Body(Entity& entity, Type type): Component(entity) {
	auto& w = entity.context().systemRef<Physics>()._world;
	b2BodyDef bd;
	bd.type = [type] {
		if (type == STATIC) {
			return b2_staticBody;
		} else if (type == DYNAMIC) {
			return b2_dynamicBody;
		} else {
			return b2_kinematicBody;
		}
	}();
	bd.position = to(entity.get<Position>()->get());

	bd.userData.entity = &entity;

	_body = w.CreateBody(&bd);
}

Body::~Body() {
	if (_body) {
		auto& w = entity().context().systemRef<Physics>()._world;
		w.DestroyBody(_body);
	}
}
