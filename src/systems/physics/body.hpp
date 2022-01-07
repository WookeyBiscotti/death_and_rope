#pragma once

#include <common/vector2.hpp>
#include <engine/component.hpp>

class b2Body;

class Body: public Component {
	friend class Collider;

  public:
	enum Type {
		STATIC,
		DYNAMIC,
		KINETIC,
	};

	explicit Body(Entity& entity, Type type);

	void position(Vector2f position);

	~Body();

  private:
	b2Body* _body{};
};