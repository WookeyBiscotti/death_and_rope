#pragma once

#include <common/vector2.hpp>
#include <engine/component.hpp>
//
#include <aabb_tree/typedefs.hpp>

class Entity;

class Collider: public Component {
	friend class Physics;

  public:
	explicit Collider(Entity& entity, const Vector2f& size, const Vector2f& origin = {});

	~Collider();

	const Vector2f& size() const { return _size; }
	void size(const Vector2f& size) { _size = size; }

	const Vector2f& origin() const { return _origin; }
	void origin(const Vector2f& origin) { _origin = origin; }

  private:
	biss::index_t _colliderIdx = biss::nullindex;

	Vector2f _size;
	Vector2f _origin;
};