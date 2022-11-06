#pragma once

#include <absl/container/inlined_vector.h>

namespace al {

template<typename T, size_t N>
using InlinedVector = absl::InlinedVector<T, N>;

}
