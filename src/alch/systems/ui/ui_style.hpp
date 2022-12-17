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
};

struct StyleValue {
	using StyleMap = al::HashMap<StyleName, StyleValue>;
	using StyleValueType = Variant<std::monostate, float, int, bool, Color, String, StyleMap>;

	StyleValueType value;
};

class Style: public EnableSharedFromThis<Style> {
  public:
	void clearAgregatedStyles();

	template<class T>
	Optional<T> get(StyleName name) {
		auto val = std::get_if<T>(_style[name].value);
		if (val) {
			return *val;
		}

		val = std::get_if<T>(_agregatedStyle[name].value);
		if (val) {
			return *val;
		}

		auto p = _parent.lock();
		if (p) {
			auto v = p->get<T>(name);
			_agregatedStyle[name] = v;

			return v;
		}

		return {};
	}

	template<class T>
	T getOr(StyleName name, T&& def) {
		return get<T>(name).value_or(std::move(def));
	}

  private:
	WeakPtr<Style> _parent;
	StyleValue::StyleMap _style;

	StyleValue::StyleMap _agregatedStyle;
};

} // namespace al
