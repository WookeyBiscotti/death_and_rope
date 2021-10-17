#pragma once

#include "scene.hpp"
#include "texture.hpp"
#include <fstream>
#include <memory>
#include <unordered_map>

class AssetCache
{
public:
    void setContext(Context* context) { _context = context; }

    std::shared_ptr<Texture> texture(const std::string& name);

    std::string readFile(const std::string& filePath);
    std::vector<uint8_t> readBinaryFile(const std::string& filePath);

    std::ifstream getBinaryFileStream(const std::string& filePath);

    std::shared_ptr<Scene> scene(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
    std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;

    Context* _context;
};