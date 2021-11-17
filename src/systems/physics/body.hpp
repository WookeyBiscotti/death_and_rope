#pragma once

#include <common/vector2.hpp>
#include <engine/component.hpp>

class Body: public Component {
  public:
	enum Type {
		STATIC,
		DYNAMIC,
	};

	explicit Body(Entity& entity, Type type = DYNAMIC, float mass = 1);
	~Body();

	Type type() const { return _type; }
	void type(Type type);

	float mass() const { return _mass; }
	void mass(float mass) { _mass = mass; }

	const Vector2f& velocity() const { return _velocity; }
	void velocity(const Vector2f& velocity) { _velocity = velocity; }

  private:
	Type _type{STATIC};
	float _mass{};
	Vector2f _velocity{};
};