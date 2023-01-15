#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/map.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/containers/vector.hpp"
#include "alch/common/variant.hpp"

namespace al {

using ConfigValue = Variant<bool, float, int64_t, String>;

} // namespace al
