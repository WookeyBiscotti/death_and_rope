#pragma once

#include <absl/container/btree_map.h>

namespace al {

template<class K, class V>
using Map = absl::btree_map<K, V>;
}
