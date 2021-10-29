#pragma once

#include <ctti/type_id.hpp>
#include <memory>
#include <unordered_map>

class Scene;
class Renderer;
class AssetCache;
namespace sf {
class RenderTarget;
}
struct Context
{
public:
    Context(AssetCache& cache, sf::RenderTarget& target, Renderer& renderer)
        : cache(cache)
        , target(target)
        , renderer(renderer)
    {}

    bool isRuning;
    std::shared_ptr<Scene> nextScene;
    AssetCache& cache;
    sf::RenderTarget& target;

    Renderer& renderer;
};