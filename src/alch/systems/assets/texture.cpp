#include "texture.hpp"

#include "alch/systems/assets/asset_cache.hpp"

bool Texture::loadFromFile(const std::string& path) {
	return _texture.loadFromFile(path);
}
