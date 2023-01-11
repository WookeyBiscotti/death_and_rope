#pragma once

#include "alch/common/containers/string.hpp"
#include "alch/common/containers/vector.hpp"
#include "alch/common/variant.hpp"

namespace al {

struct ConfigValue {
	enum Access {
		ENGINE,
		USER,
	};

	using Value = Variant<VariantEmpty, bool, float, int64_t, String>;

	Access write = ENGINE;

	Value value{VariantEmpty{}};
	// TODO:: Add observers
};

} // namespace al
