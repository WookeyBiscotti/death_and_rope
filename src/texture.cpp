#include "texture.hpp"

bool Texture::load(AssetCache&)
{
    return _texture.loadFromFile(name());
}