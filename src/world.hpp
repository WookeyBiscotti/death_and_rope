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
    friend class WorldEditor;

public:
    World() = default;

    void draw(sf::RenderTarget& window, const sf::View& camera);

    WorldCell& cellGetOrCreate(const sf::Vector2i& cell);
    WorldCell* cellGet(const sf::Vector2i& cell);

    TileInfo& tileGetOrCreate(const sf::Vector2f& p);

    template<class Archive>
    void save(Archive& ar) const
    {
        ar(_loadedCells.size());
        for (const auto& [k, v] : _loadedCells) {
            ar(k, *v);
        }
    }

    template<class Archive>
    void load(Archive& ar)
    {
        size_t s;
        ar(s);
        while (s-- != 0) {
            Vector2i k;
            WorldCell v;
            ar(k, v);
            _loadedCells.emplace(k, std::make_shared<WorldCell>(v));
        }
    }

private:
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