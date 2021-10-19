#pragma once

#include <memory>

class Scene;
class AssetCache;
namespace sf {
class RenderTarget;
}
struct Context
{
    Context(AssetCache& cache, sf::RenderTarget& target)
        : cache(cache)
        , target(target)
    {}

    bool isRuning;
    std::shared_ptr<Scene> nextScene;
    AssetCache& cache;
    sf::RenderTarget& target;
};