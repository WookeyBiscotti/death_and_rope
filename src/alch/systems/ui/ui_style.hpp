#pragma once

#include "alch/common/color.hpp"
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/optional.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/variant.hpp"
#include "alch/common/smart_ptr.hpp"

#include <cereal/types/variant.hpp>

namespace al {

enum class StyleName {
	HEADER_COLOR,
	BACKGROUND_COLOR,
	BACKGROUND_COLOR2,
	FOREGROUND_COLOR,
	FOREGROUND_COLOR2,
	BORDER_COLOR,
	BORDER_THICKNESS,

	TEXT_SIZE,
	TEXT_COLOR,
	TEXT_BOLD,

	FONT,
};

template<StyleName name, class T>
static const T styleDefault;
//https://coolors.co/palette/03045e-023e8a-0077b6-0096c7-00b4d8-48cae4-90e0ef-ade8f4-caf0f8
template<>
static const Color styleDefault<StyleName::HEADER_COLOR, Color> = Color(0xC74E00ff);
template<>
static const Color styleDefault<StyleName::BACKGROUND_COLOR, Color> = Color(0x696969);
template<>
static const Color styleDefault<StyleName::BACKGROUND_COLOR2, Color> = Color(0x808080);
template<>
static const Color styleDefault<StyleName::FOREGROUND_COLOR, Color> = Color(0x909090);
template<>
static const Color styleDefault<StyleName::FOREGROUND_COLOR2, Color> = Color(0x999999);
template<>
static const Color styleDefault<StyleName::BORDER_COLOR, Color> = Color(0x111111ff);
template<>
static const float styleDefault<StyleName::BORDER_THICKNESS, float> = 1.0f;
template<>
static const Color styleDefault<StyleName::TEXT_COLOR, Color> = Color(0xffffffff);
template<>
static const float styleDefault<StyleName::TEXT_SIZE, float> = 18.0f;
template<>
static const float styleDefault<StyleName::TEXT_BOLD, bool> = true;
template<>
static const String styleDefault<StyleName::FONT, String> = "__default__";

struct Styles {
	using Map = al::FlatMap<StyleName, Styles>;
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
