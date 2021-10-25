#pragma once

#include "vector2.hpp"
#include "world_cell.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

class Context;

class World
{
    friend class WorldEditor;

public:
    const std::string& name() const { return _name; }
    void name(const std::string& name) { _name = name; }

    void clear() { _loadedCells.clear(); }

    void draw(sf::RenderTarget& window, const sf::View& camera);

    WorldCell& cellGetOrCreate(const sf::Vector2i& cell);
    WorldCell* cellGet(const sf::Vector2i& cell);

    TileInfo& tileGetOrCreate(const sf::Vector2f& p);

    void saveToDir(const std::string& path) const;
    void loadFromDir(const std::string& path);

private:
    std::string _name;
    std::unordered_map<Vector2i, std::shared_ptr<WorldCell>> _loadedCells;

private:
    //     struct
    //     {
    //         std::atomic_bool haveNewCache;
    //         std::mutex mutex;
    //         std::vector<std::shared_ptr<WorldCell>> newCells;
    //     } cacheLoader_;
    //
};