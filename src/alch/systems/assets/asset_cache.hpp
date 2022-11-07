#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/types.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/scenes/scene.hpp"
#include "font.hpp"
#include "texture.hpp"
//
#include <SFML/Graphics/Font.hpp>
//
#include <fstream>
#include <memory>

namespace al {

class Context;

class AssetCache: public System {
  public:
	explicit AssetCache(Context& context);

	Path texturesPath() const;
	Path fontsPath() const;
	Path entityPath() const;

	SharedPtr<Texture> texture(const std::string& name);
	SharedPtr<Font> font(const std::string& name = "");

	std::string readFile(const std::string& filePath);
	std::vector<uint8_t> readBinaryFile(const std::string& filePath);

	std::vector<std::string> worlds() const;

  private:
	const std::string _root;

	HashMap<std::string, SharedPtr<Texture>> _textures;
	HashMap<std::string, SharedPtr<Scene>> _scenes;
	HashMap<std::string, SharedPtr<Font>> _fonts;
};

} // namespace al
