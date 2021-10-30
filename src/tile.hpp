#pragma once

#include <bitset>
#include <cereal/types/bitset.hpp>
#include <cinttypes>

struct TileInfo {
	std::size_t visualId;
	std::size_t typeId;
	std::bitset<32> collision;

	template<class Archive>
	void serialize(Archive& ar) {
		ar(visualId, typeId, collision);
	}
};