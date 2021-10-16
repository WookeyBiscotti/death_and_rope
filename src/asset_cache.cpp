#include "asset_cache.hpp"

#include <streambuf>
#include <string>

std::shared_ptr<Texture> AssetCache::texture(const std::string& name)
{
    if (auto found = _textures.find(name); found != _textures.end()) {
        return found->second;
    }

    auto texture = std::make_shared<Texture>(name);
    if (texture->load(*this)) {
        _textures.emplace(name, std::move(texture));
        return texture;
    }

    //return fake texture
    return nullptr;
}

std::string AssetCache::readFile(const std::string& filePath)
{
    std::ifstream t(filePath);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return str;
}

std::vector<uint8_t> AssetCache::readBinaryFile(const std::string& filePath)
{
    std::ifstream t(filePath, std::ios::binary);
    std::vector<uint8_t> data;

    t.seekg(0, std::ios::end);
    data.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    t.read(reinterpret_cast<char*>(data.data()), data.size());

    return data;
}

std::ifstream AssetCache::getBinaryFileStream(const std::string& filePath)
{
    return std::ifstream(filePath, std::ios::binary);
}