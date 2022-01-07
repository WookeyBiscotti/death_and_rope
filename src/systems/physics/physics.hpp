#pragma once

#include "collider.hpp"

#include <systems/broker/receiver.hpp>
//

#include <aabb_tree/aabb_tree.hpp>
#include <box2d/box2d.h>
//
#include <unordered_set>

class Context;
class Entity;
class Body;

class Physics: public Receiver {
	friend class Body;
	friend class Collider;

  public:
	explicit Physics(Context& context);

	void update(float dt);

	b2World& internalWorld() { return _world; }

  private:
	Context& _contex;

	b2World _world;
};