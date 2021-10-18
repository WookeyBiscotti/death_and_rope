#pragma once

#include "scene.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include <fstream>
#include <memory>
#include <unordered_map>

class AssetCache
{
public:
    void setContext(Context* context) { _context = context; }

    std::shared_ptr<Texture> texture(const std::string& name);

    std::shared_ptr<Sprite> sprite(const std::string& name);

    std::string readFile(const std::string& filePath);
    std::vector<uint8_t> readBinaryFile(const std::string& filePath);

    std::vector<uint8_t> readBinaryTextureFile(const std::string& filePath);

    std::ifstream getITextureFileStream(const std::string& filePath);
    std::ofstream getOTextureFileStream(const std::string& filePath);

    std::ifstream getISpriteFileStream(const std::string& filePath);
    std::ofstream getOSpriteFileStream(const std::string& filePath);

    std::shared_ptr<Scene> scene(const std::string& name);

    std::vector<std::string> sprites() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
    std::unordered_map<std::string, std::shared_ptr<Sprite>> _sprites;
    std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;

    Context* _context;
};