#pragma once

#include <bitset>
#include <cereal/types/bitset.hpp>

namespace al {

template<size_t N>
using Bitset = std::bitset<N>;

}

template<class Archive, size_t N>
inline void serialize(Archive& archive, al::Bitset<N>& m) {
	archive(m);
}

template<class Archive, size_t N>
inline void serialize(Archive& archive, const al::Bitset<N>& m) {
	archive(m);
}
