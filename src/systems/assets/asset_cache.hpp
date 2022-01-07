#pragma once

#include "font.hpp"
#include "texture.hpp"
#include "world.hpp"

#include <common/types.hpp>
#include <engine/system.hpp>
#include <systems/scenes/scene.hpp>
//
#include <SFML/Graphics/Font.hpp>
//
#include <fstream>
#include <memory>
#include <unordered_map>

class Context;

class AssetCache: public System {
  public:
	explicit AssetCache(Context& context);

	Path texturesPath() const;
	Path fontsPath() const;
	Path worldsPath() const;

	std::shared_ptr<Texture> texture(const std::string& name);
	std::shared_ptr<Font> font(const std::string& name);

	World world(const std::string& name);
	void world(const World& world, const std::string& name);

	std::string readFile(const std::string& filePath);
	std::vector<uint8_t> readBinaryFile(const std::string& filePath);

	std::vector<std::string> worlds() const;

  private:
	Context& _context;

	const std::string _root;

	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
	std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
	std::unordered_map<std::string, std::shared_ptr<Font>> _fonts;
};