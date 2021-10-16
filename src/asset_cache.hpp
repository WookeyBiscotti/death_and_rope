#pragma once

#include "texture.hpp"
#include <fstream>
#include <memory>
#include <unordered_map>

class AssetCache
{
public:
    std::shared_ptr<Texture> texture(const std::string& name);

    std::string readFile(const std::string& filePath);
    std::vector<uint8_t> readBinaryFile(const std::string& filePath);

    std::ifstream getBinaryFileStream(const std::string& filePath);

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
};