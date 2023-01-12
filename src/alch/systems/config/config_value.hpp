#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/map.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/containers/vector.hpp"
#include "alch/common/variant.hpp"

namespace al {

struct ConfigValue {
	enum Access {
		ENGINE,
		USER,
	};
	Access write = ENGINE;

	using Map = NodeHashMap<String, ConfigValue>;

	using Value = Variant<VariantEmpty, bool, float, int64_t, String, NodeHashMap<String, ConfigValue>>;

	Value value;

	template<class Archive>
	void serialize(Archive& ar) {
		ar(write, value);
	}
	// TODO:: Add observers
};

} // namespace al
