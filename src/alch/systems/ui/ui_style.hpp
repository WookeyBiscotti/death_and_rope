#pragma once

#include "alch/common/color.hpp"
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/optional.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/containers/variant.hpp"
#include "alch/common/smart_ptr.hpp"

namespace al {

enum class StyleName {
	BACKGROUND_COLOR,
	BORDER_COLOR,
	BORDER_THICKNESS,

	TEXT_SIZE,
	TEXT_COLOR,

	BTN_IDL_COLOR,
	BTN_PRESSED_COLOR,
	BTN_HOVERED_COLOR,
};

template<StyleName name, class T>
static const T styleDefault;

template<>
static const Color styleDefault<StyleName::BACKGROUND_COLOR, Color> = Color(50, 50, 50);
template<>
static const Color styleDefault<StyleName::BORDER_COLOR, Color> = Color::White;
template<>
static const float styleDefault<StyleName::BORDER_THICKNESS, float> = 1.0f;
template<>
static const Color styleDefault<StyleName::TEXT_COLOR, Color> = Color::White;
template<>
static const float styleDefault<StyleName::TEXT_SIZE, float> = 14.0f;
template<>
static const Color styleDefault<StyleName::BTN_IDL_COLOR, Color> = Color(50, 50, 50);
template<>
static const Color styleDefault<StyleName::BTN_HOVERED_COLOR, Color> = Color(55, 55, 55);
template<>
static const Color styleDefault<StyleName::BTN_PRESSED_COLOR, Color> = Color(35, 35, 35);

struct Styles {
	using Map = al::HashMap<StyleName, Styles>;
	using Value = Variant<float, int, bool, Color, String, Map>;

	Value value;
};

} // namespace al
