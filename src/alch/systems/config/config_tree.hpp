#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/map.hpp"
#include "config_value.hpp"

namespace al {

struct SystemConfigValue {
	enum Access {
		ENGINE,
		USER,
	} access = ENGINE;

	ConfigValue value;

	template<class Archive>
	void serialize(Archive& ar) {
		ar(access, value);
	}
};

using UserConfigValue = ConfigValue;

} // namespace al
