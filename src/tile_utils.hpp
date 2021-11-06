#pragma once

#include <common/vector2.hpp>
#include <config.hpp>

inline Vector2i tileFromPoint(Vector2f p) {
	if (p.x < 0) {
		p.x -= TILE_SIZE;
	}
	if (p.y < 0) {
		p.y -= TILE_SIZE;
	}
	return {(int)p.x / TILE_SIZE, (int)p.y / TILE_SIZE};
}

inline Vector2i worldCellFromPoint(Vector2f p) {
	auto t = tileFromPoint(p);
	if (t.x < 0) {
		t.x -= (TILES_IN_WORLD_CELL_W - 1);
	}
	if (t.y < 0) {
		t.y -= (TILES_IN_WORLD_CELL_H - 1);
	}
	return {t.x / TILES_IN_WORLD_CELL_W, t.y / TILES_IN_WORLD_CELL_H};
}

inline Vector2f worldCellFromIdx(const Vector2i& idx) {
	return {(float)idx.x * TILES_IN_WORLD_CELL_W * TILE_SIZE, (float)idx.y * TILES_IN_WORLD_CELL_H * TILE_SIZE};
}

inline Vector2i cellLocalIdxFromPoint(const Vector2f& p) {
	auto globalTileIdx = tileFromPoint(p);
	globalTileIdx = {globalTileIdx.x % TILES_IN_WORLD_CELL_W, globalTileIdx.y % TILES_IN_WORLD_CELL_H};

	if (globalTileIdx.x < 0) {
		globalTileIdx.x += TILES_IN_WORLD_CELL_W;
	}
	if (globalTileIdx.y < 0) {
		globalTileIdx.y += TILES_IN_WORLD_CELL_H;
	}

	return globalTileIdx;
}