#include "body.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/transform/transform.hpp"
#include "physics.hpp"
//
#include <box2d/box2d.h>

using namespace al;

Body::Body(Entity& entity): Body(entity, Type::STATIC) {
}

Body::Body(Entity& entity, Type type): Component(entity) {
	auto& w = entity.context().systemRef<Physics>().internalWorld();
	b2BodyDef bd;
	bd.type = [type] {
		if (type == STATIC) {
			return b2_staticBody;
		} else if (type == DYNAMIC || type == BULLET) {
			return b2_dynamicBody;
		} else {
			return b2_kinematicBody;
		}
	}();
	bd.position = to(entity.ref<Transform>().p());
	bd.bullet = type == BULLET;

	bd.userData.entity = &entity;

	_body = w.CreateBody(&bd);
}

Body::~Body() {
	auto& w = entity().context().systemRef<Physics>()._world;
	w.DestroyBody(_body);
}

void Body::position(Vector2f position) {
	_body->SetTransform(to(position), _body->GetAngle());
	entity().tr().p(position);
}

void Body::serialize(OArchive& ar) const {
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

void Body::deserialize(IArchive& ar) {
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
