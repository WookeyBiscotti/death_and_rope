#include "sprite.hpp"

#include "archive.hpp"
#include "asset_cache.hpp"
#include <fstream>

bool Sprite::loadFromFile(const std::string& path, AssetCache& cache)
{
    std::ifstream stream(path, std::ios::binary);
    IArchive archive(stream);

    std::string textureName;
    archive >> textureName;
    _texture = cache.texture(textureName);
    archive >> _textureRect;

    _sprite.setTexture(_texture->sf());
    _sprite.setTextureRect(_textureRect);

    return true;
}

bool Sprite::saveToFile(const std::string& path)
{
    std::ofstream stream(path, std::ios::binary);
    OArchive archive(stream);

    archive << _texture->name();
    archive << _textureRect;

    return true;
}

void Sprite::draw(sf::RenderTarget& target, const Vector2f& p)
{
    _sprite.setPosition(p);
    target.draw(_sprite);
}