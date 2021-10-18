#include "sprite.hpp"

#include "archive.hpp"
#include "asset_cache.hpp"

bool Sprite::load(AssetCache& cache)
{
    //TODO: add error checks
    auto stream = cache.getBinaryFileStream(name());
    IArchive archive(stream);

    archive >> _size;

    std::string textureName;
    archive >> textureName;
    _texture = cache.texture(textureName);
    archive >> _textureRect;

    _sprite.setTexture(_texture->texture());
    _sprite.setTextureRect(_textureRect);

    return true;
}

bool Sprite::save(AssetCache& cache)
{
    //TODO: add error checks
    auto stream = cache.getBinaryFileStream(name());
    IArchive archive(stream);

    archive >> _size;

    std::string textureName;
    archive >> textureName;
    _texture = cache.texture(textureName);
    archive >> _textureRect;

    _sprite.setTexture(_texture->texture());
    _sprite.setTextureRect(_textureRect);

    return true;
}

void Sprite::draw(sf::RenderTarget& target, const Vector2f& p)
{
    _sprite.setPosition(p);
    target.draw(_sprite);
}