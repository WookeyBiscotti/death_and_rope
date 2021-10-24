#pragma once

#include "asset.hpp"
#include <SFML/Graphics/Texture.hpp>

class Texture : public Asset
{
public:
    Texture(std::string name)
        : Asset(std::move(name)){};

    bool loadFromFile(const std::string& path);

    sf::Texture& texture() { return _texture; }

private:
    sf::Texture _texture;
};