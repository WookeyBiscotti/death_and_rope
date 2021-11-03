#pragma once

#include "font.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include "world.hpp"

#include <systems/scenes/scene.hpp>
#include <types.hpp>
//
#include <SFML/Graphics/Font.hpp>
//
#include <fstream>
#include <memory>
#include <unordered_map>

class Context;

class AssetCache {
  public:
	AssetCache(Context& context);

	Path spritesPath() const;
	Path texturesPath() const;
	Path fontsPath() const;
	Path worldsPath() const;

	std::shared_ptr<Texture> texture(const std::string& name);
	std::shared_ptr<Font> font(const std::string& name);
	std::shared_ptr<Sprite> sprite(const std::string& name);

	World world(const std::string& name);
	void world(const World& world, const std::string& name);

	std::string readFile(const std::string& filePath);
	std::vector<uint8_t> readBinaryFile(const std::string& filePath);

	std::shared_ptr<Scene> scene(const std::string& name);

	std::vector<std::string> sprites() const;
	std::vector<std::string> worlds() const;

  private:
	Context& _context;

	const std::string _root;

	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
	std::unordered_map<std::string, std::shared_ptr<Sprite>> _sprites;
	std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
	std::unordered_map<std::string, std::shared_ptr<Font>> _fonts;
};