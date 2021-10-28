#pragma once

#include <ctti/type_id.hpp>
#include <memory>
#include <unordered_map>

class Scene;
class AssetCache;
namespace sf {
class RenderTarget;
}
struct Context
{
public:
    Context(AssetCache& cache, sf::RenderTarget& target)
        : cache(cache)
        , target(target)
    {}

    bool isRuning;
    std::shared_ptr<Scene> nextScene;
    AssetCache& cache;
    sf::RenderTarget& target;

    // private:
    // std::unordered_map<ctti::type_id_t>
};