#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

inline float vec(const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.y - a.y * b.x;
}

inline float cross(const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.x + a.y * b.y;
}

inline sf::Vector2f mod(const sf::Vector2f& p) {
	return p / std::sqrt(p.x * p.x + p.y * p.y);
}

inline sf::Vector2f athwart(const sf::Vector2f& p) {
	return {-p.y, p.x};
}

inline float length(const sf::Vector2f& p) {
	return std::sqrt(p.x * p.x + p.y * p.y);
}

inline float angle(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2) {
	auto a = a2 - a1;
	auto b = b2 - b1;
	return vec(a, b) / (cross(a, a) * cross(b, b));
}