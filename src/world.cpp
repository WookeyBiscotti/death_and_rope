#include "world.hpp"
#include "archive.hpp"
#include "tile_utils.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>

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

    const auto lowerIdx = worldCellFromPoint(lower);
    const auto upperIdx = worldCellFromPoint(upper);

    for (int cx = lowerIdx.x; cx <= upperIdx.x; ++cx) {
        for (int cy = lowerIdx.y; cy <= upperIdx.y; ++cy) {
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

void World::saveToDir(const std::string& path) const
{
    std::filesystem::create_directory(path);
    for (const auto& [idx, cell] : _loadedCells) {
        const auto name = std::to_string(idx.x) + "_" + std::to_string(idx.y);
        std::ofstream stream(path + "/" + name);
        OArchive archive(stream);

        archive << *cell;
    }
}

void World::loadFromDir(const std::string& path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            const auto name = entry.path().filename().string();
            // Vector2i idx;
            // idx.x = std::stoi(name.substr(0, name.find('_')));
            // idx.y = std::stoi(name.substr(name.find('_') + 1));
            std::ifstream stream(path + "/" + name);
            IArchive archive(stream);
            auto cell = std::make_shared<WorldCell>();
            archive >> *cell;
            _loadedCells[cell->cellPosition] = std::move(cell);
        }
    }
}