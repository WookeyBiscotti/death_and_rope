#include "transform.hpp"

#include "events.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"

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

void Transform::serialize(OArchive& ar) const {
	::serialize(ar, _position);
	::serialize(ar, _size);
	ar(_rotation);
}

void Transform::deserialize(IArchive& ar) {
	::serialize(ar, _position);
	::serialize(ar, _size);
	ar(_rotation);
}
