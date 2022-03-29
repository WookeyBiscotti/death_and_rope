#pragma once

#include "alch/common/archive.hpp"

#include <SFML/Graphics/Color.hpp>

using Color = sf::Color;

inline void serialize(OArchive& archive, const Color& c) {
	archive(c.r, c.g, c.b, c.a);
}

inline void serialize(IArchive& archive, Color& c) {
	archive(c.r, c.g, c.b, c.a);
}
