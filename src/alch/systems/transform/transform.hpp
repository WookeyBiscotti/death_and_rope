#pragma once

#include <common/archive.hpp>
#include "alch/common/vector2.hpp"
#include <engine/component.hpp>
//

class Transform: public Component {
  public:
	explicit Transform(Entity& entity, const Vector2f& vec = {}): Component(entity), _position(vec){};
	explicit Transform(Entity& entity, float x, float y): Component(entity), _position({x, y}){};

	const Vector2f& position() const { return _position; }
	const Vector2f& p() const { return position(); }

	void position(const Vector2f& position);
	void p(const Vector2f& position) { this->position(position); }

	const Vector2f& size() const { return _size; }
	const Vector2f& s() const { return size(); }

	void size(const Vector2f& size);
	void s(const Vector2f& size) { this->size(size); }

	float rotation() const { return _rotation; }
	float r() const { return rotation(); }

	void rotation(float rotation);
	void r(float rotation) { this->rotation(rotation); }

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

  private:
	Vector2f _position{};
	Vector2f _size{1, 1};
	float _rotation{};
};
