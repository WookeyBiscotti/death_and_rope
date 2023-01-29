#include "transform.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "events.hpp"

using namespace al;

void Transform::position(const Vector2f& position) {
	if (_position != position) {
		PositionUpdate e{_position, position};
		entity().send(e);
		_position = position;
	}
}

void Transform::size(const Vector2f& size) {
	if (_size != size) {
		SizeUpdate e{_size, size};
		entity().send(e);
		_size = size;
	}
}

void Transform::rotation(float rotation) {
	if (_rotation != rotation) {
		RotationUpdate e{_rotation, rotation};
		entity().send(e);
		_rotation = rotation;
	}
}

void Transform::save(VarOArchive& archive) const {
	al::save(archive, _position, _size, _rotation);
}

void Transform::load(VarIArchive& archive) {
	al::load(archive, _position, _size, _rotation);
}
