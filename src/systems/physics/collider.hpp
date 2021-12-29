#pragma once

#include "body.hpp"

#include <common/vector2.hpp>
#include <engine/component.hpp>
//

class Entity;
class b2Fixture;
class Collider: public Component {
	friend class Physics;

  public:
	struct Box_t {};
	constexpr static auto Box = Box_t{};

	~Collider();

	Collider(Collider&& other): Component(other.entity()) {
		_fixture = other._fixture;
		other._fixture = nullptr;
	}

	Collider(Entity& entity, Box_t, const Vector2f& size, float density = 1, const Vector2f& origin = {});

  private:
	b2Fixture* _fixture{};
};