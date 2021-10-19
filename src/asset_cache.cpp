#include "asset_cache.hpp"

#include <streambuf>
#include <string>

#include <filesystem>
#include <scenes/dev_menu.hpp>
#include <scenes/main_menu.hpp>
#include <scenes/sprite_editor.hpp>
#include <scenes/sprites_view.hpp>
#include <scenes/world_editor.hpp>

constexpr auto TEXTURE_PATH = "assets/textures/";
constexpr auto SPRITE_PATH = "assets/sprites/";

std::shared_ptr<Texture> AssetCache::texture(const std::string& name)
{
    if (auto found = _textures.find(name); found != _textures.end()) {
        return found->second;
    }

    auto texture = std::make_shared<Texture>(name);
    if (texture->load(*this)) {
        _textures.emplace(name, texture);
        return texture;
    }

    //return fake texture
    return nullptr;
}

std::shared_ptr<Sprite> AssetCache::sprite(const std::string& name)
{
    if (auto found = _sprites.find(name); found != _sprites.end()) {
        return found->second;
    }

    auto sprite = std::make_shared<Sprite>(name);
    if (sprite->load(*this)) {
        _sprites.emplace(name, sprite);
        return sprite;
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
    data.resize(t.tellg());
    t.seekg(0, std::ios::beg);

    t.read(reinterpret_cast<char*>(data.data()), data.size());

    return data;
}

std::vector<uint8_t> AssetCache::readBinaryTextureFile(const std::string& filePath)
{
    return readBinaryFile(TEXTURE_PATH + filePath);
}

std::ifstream AssetCache::getITextureFileStream(const std::string& filePath)
{
    return std::ifstream(TEXTURE_PATH + filePath, std::ios::binary);
}

std::ofstream AssetCache::getOTextureFileStream(const std::string& filePath)
{
    return std::ofstream(TEXTURE_PATH + filePath, std::ios::binary);
}

std::ifstream AssetCache::getISpriteFileStream(const std::string& filePath)
{
    return std::ifstream(SPRITE_PATH + filePath, std::ios::binary);
}

std::ofstream AssetCache::getOSpriteFileStream(const std::string& filePath)
{
    return std::ofstream(SPRITE_PATH + filePath, std::ios::binary);
}

std::shared_ptr<Scene> AssetCache::scene(const std::string& name)
{
    if (name == "main_menu") {
        if (_scenes.contains(name)) {
            return _scenes[name];
        }
        else {
            auto scene = std::make_shared<MainMenu>(*_context);
            _scenes.emplace(name, scene);

            return scene;
        }
    }
    else if (name == "dev_menu") {
        if (_scenes.contains(name)) {
            return _scenes[name];
        }
        else {
            auto scene = std::make_shared<DevMenu>(*_context);
            _scenes.emplace(name, scene);

            return scene;
        }
    }
    else if (name == "sprite_editor") {
        if (_scenes.contains(name)) {
            return _scenes[name];
        }
        else {
            auto scene = std::make_shared<SpriteEditor>(*_context);
            _scenes.emplace(name, scene);

            return scene;
        }
    }
    else if (name == "sprites_view") {
        if (_scenes.contains(name)) {
            return _scenes[name];
        }
        else {
            auto scene = std::make_shared<SpritesView>(*_context);
            _scenes.emplace(name, scene);

            return scene;
        }
    }
    else if (name == "world_editor") {
        if (_scenes.contains(name)) {
            return _scenes[name];
        }
        else {
            auto scene = std::make_shared<WorldEditor>(*_context);
            _scenes.emplace(name, scene);

            return scene;
        }
    }

    return nullptr;
}

std::vector<std::string> AssetCache::sprites() const
{
    std::vector<std::string> result;
    for (const auto& entry : std::filesystem::directory_iterator(SPRITE_PATH)) {
        if (entry.is_regular_file()) {
            result.push_back(entry.path().filename().string());
        }
    }

    return result;
}