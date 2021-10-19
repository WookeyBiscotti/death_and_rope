#include "world.hpp"
#include "tile_utils.hpp"
#include <SFML/Graphics.hpp>

TileInfo& World::tileGetOrCreate(const sf::Vector2f& p)
{
    auto& cell = cellGetOrCreate(worldCellFromPoint(p));
    const auto idx = cellLocalIdxFromPoint(p);
    return cell.tiles[idx.x][idx.y];
}

WorldCell& World::cellGetOrCreate(const sf::Vector2i& cellIdx)
{
    if (auto found = cellGet(cellIdx); found) {
        return *found;
    }

    auto& newCell = _loadedCells[cellIdx];
    newCell = std::make_shared<WorldCell>();
    newCell->cellPosition = cellIdx;
    return *newCell;
}

WorldCell* World::cellGet(const sf::Vector2i& cell)
{
    if (auto found = _loadedCells.find(cell); found != _loadedCells.end()) {
        return found->second.get();
    }

    return nullptr;
}

void World::draw(sf::RenderTarget& window, const sf::View& camera)
{
    const auto size = camera.getSize();
    const auto position = camera.getCenter();
    const auto lower = position - 0.5f * size;
    const auto upper = lower + size;

    const auto lowerIdxX = lower.x / (TILE_SIZE * TILES_IN_WORLD_CELL_W);
    const auto lowerIdxY = lower.y / (TILE_SIZE * TILES_IN_WORLD_CELL_H);
    const auto upperIdxX = upper.x / (TILE_SIZE * TILES_IN_WORLD_CELL_W);
    const auto upperIdxY = upper.y / (TILE_SIZE * TILES_IN_WORLD_CELL_H);

    for (int cx = lowerIdxX; cx <= upperIdxX; ++cx) {
        for (int cy = lowerIdxY; cy <= upperIdxY; ++cy) {
            const auto cell = cellGet({ cx, cy });
            if (!cell) {
                continue;
            }
            const auto baseX = cx * TILES_IN_WORLD_CELL_W * TILE_SIZE;
            const auto baseY = cy * TILES_IN_WORLD_CELL_H * TILE_SIZE;
            for (int x = 0; x != TILES_IN_WORLD_CELL_W; ++x) {
                for (int y = 0; y != TILES_IN_WORLD_CELL_H; ++y) {
                    if (cell->tiles[x][y].typeId != 0) {
                        sf::RectangleShape rec;
                        rec.setPosition(baseX + TILE_SIZE * x, baseY + TILE_SIZE * y);
                        rec.setSize({ TILE_SIZE, TILE_SIZE });
                        window.draw(rec);
                    }
                }
            }
        }
    }
}