#include "texture.hpp"
#include "asset_cache.hpp"

bool Texture::load(AssetCache& cache)
{
    auto data = cache.readBinaryTextureFile(name());
    return _texture.loadFromMemory(data.data(), data.size());
}

bool Texture::save(AssetCache& cache)
{
    return true;
}