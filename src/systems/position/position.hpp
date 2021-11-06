#pragma once

#include <common/vector2.hpp>
#include <engine/component.hpp>

class Position: public Component {
  public:
	Position(Entity& entity): Component(entity){};

	const Vector2f& operator()() const { return _position; }
	const Vector2f& get() const { return _position; }

	void operator()(const Vector2f& position) { _position = position; }
	void set(const Vector2f& position) { _position = position; }

  private:
	Vector2f _position{};
};