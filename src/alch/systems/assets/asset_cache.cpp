#include "asset_cache.hpp"

#include "alch/engine/engine.hpp"
#include "alch/systems/config/config.hpp"
#include "alch/systems/logging/logger.hpp"
#include "builtin_font.hpp"
//
#include "alch/common/types.hpp"
//
#include <filesystem>
#include <streambuf>
#include <string>
#include <unordered_map>

using namespace al;

namespace fs = std::filesystem;

// static const Path TEXTURES_PATH = "assets/textures/";
// static const Path FONTS_PATH = "assets/fonts/";
// static const Path ENTITY_PATH = "assets/entity/";

static const Path TEXTURES_PATH = "";
static const Path FONTS_PATH = "";
static const Path ENTITY_PATH = "";

static const std::string DEFAULT_FONT = "__default__";

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
	if (name == "") {
		return font(DEFAULT_FONT);
	}
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

	if (auto found = _fonts.find(DEFAULT_FONT); found == _fonts.end()) {
		auto [data, size] = builtinFontMemory();
		auto font = std::make_shared<Font>(DEFAULT_FONT);
		font->sf().loadFromMemory(data, size);
		_fonts.emplace(DEFAULT_FONT, std::move(font));
	}
	LERR("Can't find font: {}, load default: {}", name, DEFAULT_FONT);

	return _fonts[DEFAULT_FONT];
}
