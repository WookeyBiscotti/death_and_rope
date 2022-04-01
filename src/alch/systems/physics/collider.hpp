#pragma once

#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"
#include "body.hpp"
//

class Entity;
class b2Fixture;
class Collider: public Component {
	friend class Physics;

  public:
	struct Box_t {};
	struct Circle_t {};
	constexpr static auto Box = Box_t{};
	constexpr static auto Circle = Circle_t{};

	Collider(Entity& entity);

	Collider(Collider&& other): Component(other.entity()) { _fixtures = std::move(other._fixtures); }
	auto& operator=(Collider&& other) {
		_fixtures = std::move(other._fixtures);
		return *this;
	};
	auto& operator=(const Collider& other) = delete;
	Collider(const Collider& other) = delete;

	Collider(Entity& entity, Box_t, const Vector2f& size, float density = 1, const Vector2f& origin = {});
	Collider(Entity& entity, Circle_t, float r, float density = 1, const Vector2f& origin = {});

	~Collider();

	void add(Box_t, const Vector2f& size, float density = 1, const Vector2f& origin = {});
	void add(Circle_t, float r, float density = 1, const Vector2f& origin = {});

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

  private:
	std::vector<b2Fixture*> _fixtures;
};
