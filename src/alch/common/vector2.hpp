#pragma once

#include <common/archive.hpp>
//
#include <SFML/System/Vector2.hpp>
#include <box2d/b2_math.h>
//
#include <cmath>
#include <functional>

template<class T>
using Vector2 = sf::Vector2<T>;

using Vector2f = sf::Vector2f;
using Vector2u = sf::Vector2u;
using Vector2i = sf::Vector2i;

inline Vector2f from(const b2Vec2& vec) {
	return {vec.x, vec.y};
}

inline b2Vec2 to(const Vector2f& vec) {
	return {vec.x, vec.y};
}

inline float vec(const Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.y - a.y * b.x;
}

inline float cross(const Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.x + a.y * b.y;
}

inline Vector2f mod(const Vector2f& p) {
	return p / std::sqrt(p.x * p.x + p.y * p.y);
}

inline Vector2f athwart(const Vector2f& p) {
	return {-p.y, p.x};
}

inline float length(const Vector2f& p) {
	return std::sqrt(p.x * p.x + p.y * p.y);
}

inline float sin(const Vector2f& a1, const Vector2f& a2, const Vector2f& b1, const Vector2f& b2) {
	auto a = a2 - a1;
	auto b = b2 - b1;
	return vec(a, b) / (cross(a, a) * cross(b, b));
}

template<class Archive, class T>
inline void serialize(Archive& archive, Vector2<T>& m) {
	archive(m.x, m.y);
}

template<class Archive, class T>
inline void serialize(Archive& archive, const Vector2<T>& m) {
	archive(m.x, m.y);
}

template<class Archive>
inline void serialize(Archive& archive, b2Vec2& m) {
	archive(m.x, m.y);
}
template<class Archive>
inline void serialize(Archive& archive, const b2Vec2& m) {
	archive(m.x, m.y);
}

namespace std {
template<class T>
struct hash<sf::Vector2<T>> {
	std::size_t operator()(const sf::Vector2<T>& k) const {
		using std::hash;
		return hash<T>()(k.x) ^ (hash<T>()(k.y) << 1);
	}
};
} // namespace std