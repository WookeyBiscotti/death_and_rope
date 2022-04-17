#include "asset_cache.hpp"

#include "alch/systems/config/config.hpp"
#include "alch/systems/logging/logger.hpp"
#include "alch/engine/engine.hpp"
//
#include "alch/common/types.hpp"
//
#include <filesystem>
#include <streambuf>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

// static const Path TEXTURES_PATH = "assets/textures/";
// static const Path FONTS_PATH = "assets/fonts/";
// static const Path ENTITY_PATH = "assets/entity/";

static const Path TEXTURES_PATH = "";
static const Path FONTS_PATH = "";
static const Path ENTITY_PATH = "";

static const std::string DEFAULT_FONT = "UbuntuMono-Bold.ttf";

static const std::unordered_map<std::string, std::string> FONT_DEFAULTS_MAP = {
    {"default", DEFAULT_FONT},
};

Path AssetCache::entityPath() const {
	return fs::path(_root) / ENTITY_PATH;
}

Path AssetCache::fontsPath() const {
	return fs::path(_root) / FONTS_PATH;
}

Path AssetCache::texturesPath() const {
	return fs::path(_root) / TEXTURES_PATH;
}

AssetCache::AssetCache(Context& context): _context(context), _root(context.engine.config().root) {
	LINFO("Asset cache root: {}", _root);
};

std::shared_ptr<Texture> AssetCache::texture(const std::string& name) {
	LDEBUG("Try to load: {} texture", name);
	if (auto found = _textures.find(name); found != _textures.end()) {
		LDEBUG("Texture {} found in  cache", name);
		return found->second;
	}

	auto texture = std::make_shared<Texture>(name);
	if (texture->loadFromFile(texturesPath() / name)) {
		LDEBUG("Texture {} loaded from disc", name);
		_textures.emplace(name, texture);
		return texture;
	}

	LERR("Texture {} not found", name);

	return nullptr;
}

std::string AssetCache::readFile(const std::string& filePath) {
	std::ifstream t(filePath);
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	return str;
}

std::vector<uint8_t> AssetCache::readBinaryFile(const std::string& filePath) {
	std::ifstream t(filePath, std::ios::binary);
	std::vector<uint8_t> data;

	t.seekg(0, std::ios::end);
	data.resize(t.tellg());
	t.seekg(0, std::ios::beg);

	t.read(reinterpret_cast<char*>(data.data()), data.size());

	return data;
}

std::shared_ptr<Font> AssetCache::font(const std::string& name) {
	if (auto found = _fonts.find(name); found != _fonts.end()) {
		LINFO("Load font from cache: {}", name);

		return found->second;
	}

	auto font = std::make_shared<Font>(name);
	if (font->loadFromFile(fontsPath() / name)) {
		LINFO("Load font to cache: {}", name);
		_fonts.emplace(name, font);

		return font;
	} else {
		LINFO("Can't load font file: {}. Try to check defaults", name);
	}

	if (auto found = FONT_DEFAULTS_MAP.find(name); found != FONT_DEFAULTS_MAP.end()) {
		if (!font->loadFromFile(fontsPath() / found->second)) {
			LINFO("Can't load defaut font: {}: {}", found->first, found->second);
		} else {
			LINFO("Load defaut font: {}: {}", found->first, found->second);
		}

		_fonts.emplace(name, font);

		return font;
	} else {
		LERR("Can't find font: {}, load default: {}", name, DEFAULT_FONT);

		return AssetCache::font(DEFAULT_FONT);
	}
}
