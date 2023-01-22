#pragma once

#include <absl/container/flat_hash_set.h>

namespace al {

template<class V>
using FlatSet = absl::flat_hash_set<V>;

}
