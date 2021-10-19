#pragma once

#include <config.hpp>
#include <vector2.hpp>

inline Vector2i tileFromPoint(const Vector2f& p)
{
    return { (int)p.x / TILE_SIZE, (int)p.y / TILE_SIZE };
}

inline Vector2i worldCellFromPoint(const Vector2f& p)
{
    return { (int)(p.x / TILE_SIZE) / TILES_IN_WORLD_CELL_W, (int)(p.y / TILE_SIZE) / TILES_IN_WORLD_CELL_H };
}

inline Vector2f worldCellFromIdx(const Vector2i& idx)
{
    return { (float)idx.x * TILES_IN_WORLD_CELL_W * TILE_SIZE, (float)idx.y * TILES_IN_WORLD_CELL_H * TILE_SIZE };
}

inline Vector2i cellLocalIdxFromPoint(const Vector2f& p)
{
    const auto globalTileIdx = tileFromPoint(p);
    return { globalTileIdx.x % TILES_IN_WORLD_CELL_W, globalTileIdx.y % TILES_IN_WORLD_CELL_H };
}