#pragma once

class b2Body;

namespace al {

class Entity;

struct PhysicsBeginContact {
	Entity& ea;
	b2Body& ba;
	Entity& eb;
	b2Body& bb;
};

}
