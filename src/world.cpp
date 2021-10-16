#include "world.hpp"

const WorldCell& World::getCell(const sf::Vector2i& cell)
{}

void World::draw(sf::RenderWindow& window, const sf::View& camera)
{
    const auto size = camera.getSize();
    const auto position = camera.getCenter();
    const auto lower = position - 0.5f * size;
    const auto upper = lower + size;

    const auto lowerIdxX = lower.x / (TILE_SIZE * TILES_IN_WORLD_CELL_W);
    const auto lowerIdxY = lower.y / (TILE_SIZE * TILES_IN_WORLD_CELL_H);
    const auto upperIdxX = upper.x / (TILE_SIZE * TILES_IN_WORLD_CELL_W);
    const auto upperIdxY = upper.y / (TILE_SIZE * TILES_IN_WORLD_CELL_H);

    for (auto x = lowerIdxX; x <= upperIdxX; ++x) {
        for (auto y = lowerIdxY; y <= upperIdxY; ++y) {
            const auto& cell = getCell({ x, y });
        }
    }
}