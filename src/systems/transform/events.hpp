#pragma once

#include <common/vector2.hpp>

struct PositionUpdate {
	Vector2f old;
	Vector2f neW;
};

struct SizeUpdate {
	Vector2f old;
	Vector2f neW;
};

struct RotationUpdate {
	float old;
	float neW;
};