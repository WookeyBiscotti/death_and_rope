#pragma once

#include <absl/container/flat_hash_set.h>

namespace al {

template<class V>
using HashSet = absl::flat_hash_set<V>;

}
