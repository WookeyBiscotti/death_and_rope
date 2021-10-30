#pragma once

#include <functional>
#include <memory>
#include <type_id.hpp>
#include <unordered_map>

class Scene;
class Renderer;
class AssetCache;
namespace sf {
class RenderTarget;
}
class Context
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

    template<class S>
    void addSystem(S* s)
    {
        _systems.emplace(TypeId<S>(), s);
    }

    template<class S>
    S* system()
    {
        if (const auto found = _systems.find(TypeId<S>()); found != _systems.end()) {
            return static_cast<S*>(found.second);
        }
        return nullptr;
    }

private:
    std::unordered_map<type_id_t, void*> _systems;
};