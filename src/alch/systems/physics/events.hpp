#pragma once

class Entity;
class b2Body;

struct PhysicsBeginContact {
	Entity& ea;
	b2Body& ba;
	Entity& eb;
	b2Body& bb;
};
