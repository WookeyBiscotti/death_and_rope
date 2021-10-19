#pragma once

#include "asset.hpp"
#include <SFML/Graphics/Texture.hpp>

class Texture : public Asset
{
public:
    Texture(std::string name)
        : Asset(std::move(name)){};

    bool load(AssetCache&) override;
    bool save(AssetCache& cache) override;

    sf::Texture& texture() { return _texture; }

private:
    sf::Texture _texture;
};