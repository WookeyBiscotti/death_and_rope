#pragma once

#include "vector2.hpp"
#include "world_cell.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

class World
{
public:
    void draw(sf::RenderWindow& window, const sf::View& camera);

    const WorldCell& getCell(const sf::Vector2i& cell);

private:
    std::unordered_map<Vector2u, std::shared_ptr<WorldCell>> _loadedCells;

private:
    struct
    {
        std::atomic_bool haveNewCache;
        std::mutex mutex;
        std::vector<std::shared_ptr<WorldCell>> newCells;
    } cacheLoader_;
};