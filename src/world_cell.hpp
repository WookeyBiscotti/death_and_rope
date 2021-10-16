#pragma once

#include "config.hpp"
#include "tile.hpp"
#include "vector2.hpp"
#include <array>

struct WorldCell
{
    Vector2i cellPosition;
    std::array<std::array<TileInfo, TILES_IN_WORLD_CELL_W>, TILES_IN_WORLD_CELL_H> tiles;

    template<class Archive>
    void serialize(Archive& ar)
    {
        ar(cellPosition, tiles);
    }
}