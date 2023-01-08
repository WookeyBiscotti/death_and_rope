#pragma once

#include "alch/common/color.hpp"
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/optional.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/containers/variant.hpp"
#include "alch/common/smart_ptr.hpp"

#include <cereal/types/variant.hpp>

namespace al {

enum class StyleName {
	BACKGROUND_COLOR,
	FOREGROUND_COLOR,
	BORDER_COLOR,
	BORDER_THICKNESS,

	TEXT_SIZE,
	TEXT_COLOR,
	TEXT_BOLD,

	BTN_IDL_COLOR,
	BTN_PRESSED_COLOR,
	BTN_HOVERED_COLOR,

	FONT,
};

template<StyleName name, class T>
static const T styleDefault;

template<>
static const Color styleDefault<StyleName::BACKGROUND_COLOR, Color> = Color(50, 50, 50);
template<>
static const Color styleDefault<StyleName::FOREGROUND_COLOR, Color> = Color(75, 75, 75);
template<>
static const Color styleDefault<StyleName::BORDER_COLOR, Color> = Color::White;
template<>
static const float styleDefault<StyleName::BORDER_THICKNESS, float> = 1.0f;
template<>
static const Color styleDefault<StyleName::TEXT_COLOR, Color> = Color::White;
template<>
static const float styleDefault<StyleName::TEXT_SIZE, float> = 14.0f;
template<>
static const float styleDefault<StyleName::TEXT_BOLD, bool> = true;
template<>
static const Color styleDefault<StyleName::BTN_IDL_COLOR, Color> = Color(75, 75, 75);
template<>
static const Color styleDefault<StyleName::BTN_HOVERED_COLOR, Color> = Color(85, 85, 85);
template<>
static const Color styleDefault<StyleName::BTN_PRESSED_COLOR, Color> = Color(50, 50, 50);
template<>
static const String styleDefault<StyleName::FONT, String> = "__default__";

struct Styles {
	using Map = al::HashMap<StyleName, Styles>;
	using Value = Variant<float, int, bool, Color, String, Map>;

	Value value;
};

} // namespace al

template<class Archive>
inline void serialize(Archive& archive, const al::Styles::Map& m) {
	archive(m.size());
	for (const auto& [k, v] : m) {
		archive(k, v);
	}
}

template<class Archive>
inline void serialize(Archive& archive, al::Styles::Map& m) {
	std::remove_cvref_t<decltype(m.size())> size;
	archive(size);
	m.reserve(size);

	using KeyType = std::remove_cvref_t<decltype(m.begin()->first)>;
	using ValueType = std::remove_cvref_t<decltype(m.begin()->second)>;

	while (size-- != 0) {
		KeyType k;
		archive(k);
		ValueType v;
		archive(v);
		m.emplace(std::move(k), std::move(v));
	}
}

template<class Archive>
inline void serialize(Archive& archive, const al::Styles& m) {
	archive(m.value);
}

template<class Archive>
inline void serialize(Archive& archive, al::Styles& m) {
	archive(m.value);
}
