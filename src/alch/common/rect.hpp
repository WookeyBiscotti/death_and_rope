#pragma once

#include "alch/common/vector2.hpp"
//
#include <SFML/Graphics/Rect.hpp>

template<class T>
using Rect = sf::Rect<T>;

using Recti = sf::IntRect;
using Rectf = sf::FloatRect;

template<class Archive, class T>
void save(Archive& archive, const Rect<T>& m) {
	archive(m.left, m.top, m.width, m.height);
}

template<class Archive, class T>
void load(Archive& archive, Rect<T>& m) {
	archive(m.left, m.top, m.width, m.height);
}

template<class T>
void move(Rect<T>& r, const sf::Vector2<T>& v) {
	r.left += v.x;
	r.top += v.y;
}
