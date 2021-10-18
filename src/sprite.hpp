#pragma once

#include "asset.hpp"
#include "rect.hpp"
#include "texture.hpp"
#include "vector2.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Sprite : public Asset
{
    friend class SpriteEditor;

public:
    Sprite(std::string name)
        : Asset(std::move(name)){};

    bool load(AssetCache& cache) override;
    bool save(AssetCache& cache) override;

    void draw(sf::RenderTarget& target, const Vector2f& p);

    sf::Sprite& sprite() { return _sprite; }

private:
    Vector2i _size;

    sf::Sprite _sprite;
    std::shared_ptr<Texture> _texture;
    Recti _textureRect;
};