#pragma once

#include "alch/engine/system.hpp"
#include "alch/systems/broker/receiver.hpp"
#include "collider.hpp"
//

#include <aabb_tree/aabb_tree.hpp>
#include <box2d/box2d.h>
//
#include <unordered_map>
#include <unordered_set>

namespace al {

class Context;
class Entity;
class Body;
class ContactListener;

struct CastRayResult {
	Entity* entity;
};

class Physics: public System {
	friend class Body;
	friend class Collider;
	friend class ContactListener;

  public:
	explicit Physics(Context& context);
	~Physics();

	void update(float dt);

	b2World& internalWorld() { return _world; }

	void castRay(Vector2f start, Vector2f end, const std::function<bool(const CastRayResult& result)>& callback);

  private:
	b2World _world;

	std::unique_ptr<ContactListener> _contactListner;
};

} // namespace al
