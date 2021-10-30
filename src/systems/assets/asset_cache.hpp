#pragma once

#include "scene.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include "world.hpp"
//
#include <SFML/Graphics/Font.hpp>
//
#include <fstream>
#include <memory>
#include <unordered_map>

class Context;

class AssetCache
{
public:
    AssetCache(Context& context)
        : _context(context)
    {}

    static const std::string& defaultSpritePath();
    static const std::string& defaultWorldsPath();

    std::shared_ptr<Texture> texture(const std::string& name);

    std::shared_ptr<sf::Font> font(const std::string& name);

    std::shared_ptr<Sprite> sprite(const std::string& name);

    World world(const std::string& name);
    void world(const World& world, const std::string& name);

    std::string readFile(const std::string& filePath);
    std::vector<uint8_t> readBinaryFile(const std::string& filePath);

    std::shared_ptr<Scene> scene(const std::string& name);

    std::vector<std::string> sprites() const;
    std::vector<std::string> worlds() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
    std::unordered_map<std::string, std::shared_ptr<Sprite>> _sprites;
    std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> _fonts;

    Context& _context;
};