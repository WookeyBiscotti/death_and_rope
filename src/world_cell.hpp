#pragma once

#include "config.hpp"
#include "tile.hpp"

#include <array>
#include <cereal/types/array.hpp>
#include <common/vector2.hpp>

struct WorldCell {
	Vector2i cellPosition;
	std::array<std::array<TileInfo, TILES_IN_WORLD_CELL_W>, TILES_IN_WORLD_CELL_H> tiles;

	template<class Archive>
	void serialize(Archive& ar) {
		ar(cellPosition, tiles);
	}
};