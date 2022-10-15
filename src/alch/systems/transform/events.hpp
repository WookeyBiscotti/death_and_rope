#pragma once

#include "alch/common/vector2.hpp"

namespace al {

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
}
