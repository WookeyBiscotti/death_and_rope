#pragma once

#include <absl/container/inlined_vector.h>
#include <cinttypes>

namespace al {

template<typename T, std::size_t N>
using InlinedVector = absl::InlinedVector<T, N>;

}
