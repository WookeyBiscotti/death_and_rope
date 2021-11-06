#pragma once

#include <common/vector2.hpp>
//
#include <SFML/Graphics/Rect.hpp>

using Recti = sf::IntRect;
using Rectf = sf::FloatRect;

template<class Archive, class T>
void serialize(Archive& archive, sf::Rect<T>& m) {
	archive(m.left, m.top, m.width, m.height);
}