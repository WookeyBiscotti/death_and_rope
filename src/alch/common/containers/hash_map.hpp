#pragma once

#include <absl/container/flat_hash_map.h>

namespace al {

template<class K, class V>
using HashMap = absl::flat_hash_map<K, V>;

}
