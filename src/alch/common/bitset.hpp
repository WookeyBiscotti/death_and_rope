#pragma once

#include <bitset>
#include <cereal/types/bitset.hpp>

namespace al {

template<size_t N>
using Bitset = std::bitset<N>;

}
