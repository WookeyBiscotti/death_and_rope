#pragma once

#include <memory>

class Scene;
class AssetCache;
struct Context
{
    Context(AssetCache& cache)
        : cache(cache)
    {}

    bool isRuning;
    std::shared_ptr<Scene> nextScene;
    AssetCache& cache;
};