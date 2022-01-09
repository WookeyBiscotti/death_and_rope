#include "body.hpp"

#include "physics.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <systems/transform/transform.hpp>
//
#include <box2d/box2d.h>

Body::Body(Entity& entity, Type type): Component(entity) {
	auto& w = entity.context().systemRef<Physics>().internalWorld();
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
	bd.position = to(entity.ref<Transform>().p());

	bd.userData.entity = &entity;

	_body = w.CreateBody(&bd);
}

Body::~Body() {
	if (_body) {
		auto& w = entity().context().systemRef<Physics>()._world;
		w.DestroyBody(_body);
	}
}

void Body::position(Vector2f position) {
	if (_body) {
		_body->SetTransform(to(position), _body->GetAngle());
	}
}

template<>
void Body::serialize(OArchive& ar) {
	ar(!!_body);

	if (_body) {
		ar(                              //
		    _body->GetType(),            //
		    _body->GetPosition(),        //
		    _body->GetAngle(),           //
		    _body->GetLinearVelocity(),  //
		    _body->GetAngularVelocity(), //
		    _body->GetAngularDamping(),  //
		    _body->IsSleepingAllowed(),  //
		    _body->IsAwake(),            //
		    _body->IsFixedRotation(),    //
		    _body->IsBullet(),           //
		    _body->IsEnabled(),          //
		    _body->GetGravityScale()     //
		);
	}
}

template<>
void Body::serialize(IArchive& ar) {
	bool inited;
	ar(inited);

	if (inited) {
		b2BodyDef bd;
		ar(                     //
		    bd.type,            //
		    bd.position,        //
		    bd.angle,           //
		    bd.linearVelocity,  //
		    bd.angularVelocity, //
		    bd.angularDamping,  //
		    bd.allowSleep,      //
		    bd.awake,           //
		    bd.fixedRotation,   //
		    bd.bullet,          //
		    bd.enabled,         //
		    bd.gravityScale     //
		);
		bd.userData.entity = &entity();
		auto& w = entity().context().systemRef<Physics>().internalWorld();
		_body = w.CreateBody(&bd);
	}
}