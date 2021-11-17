#pragma once

#include "collider.hpp"

#include <systems/broker/receiver.hpp>
//
#include <aabb_tree/aabb_tree.hpp>
//
#include <unordered_set>

class Context;
class Entity;
class Body;

class Physics: public Receiver {
  public:
	explicit Physics(Context& context);

	void add(Collider* collider);
	void remove(Collider* collider);

	void add(Body* body);
	void remove(Body* body);

	void update(float dt);

  private:
	Context& _contex;

	using AABBTree = biss::AABBTree<Collider*, 2, float>;
	AABBTree _colliders;

	std::unordered_set<Body*> _dynamic;
	std::unordered_set<Body*> _static;
};