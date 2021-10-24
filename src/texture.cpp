#include "texture.hpp"
#include "asset_cache.hpp"

bool Texture::loadFromFile(const std::string& path)
{
    return _texture.loadFromFile(path);
}
