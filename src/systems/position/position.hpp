#pragma once

#include <common/vector2.hpp>
#include <engine/component.hpp>

class Position: public Component {
  public:
	explicit Position(Entity& entity, const Vector2f& vec = {}): Component(entity), _position(vec){};
	explicit Position(Entity& entity, float x, float y): Component(entity), _position({x, y}){};

	const Vector2f& operator()() const { return get(); }
	const Vector2f& get() const { return _position; }

	void operator()(const Vector2f& position) { set(position); }
	void set(const Vector2f& position);

  private:
	Vector2f _position{};
};