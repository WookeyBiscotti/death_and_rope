#pragma once

#include <SFML/Graphics/Color.hpp>

using Color = sf::Color;

template<class Archive>
void save(Archive& archive, const Color& c) {
	archive(c.r, c.g, c.b, c.a);
}

template<class Archive>
void load(Archive& archive, Color& c) {
	archive(c.r, c.g, c.b, c.a);
}
