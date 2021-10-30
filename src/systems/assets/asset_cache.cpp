#include <scenes/dev_menu.hpp>
#include <scenes/main_menu.hpp>
#include <scenes/sprite_editor.hpp>
#include <scenes/sprites_view.hpp>
#include <scenes/test.hpp>
#include <scenes/world_editor.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/logging/logger.hpp>
//
#include <archive.hpp>
//
#include <filesystem>
#include <streambuf>
#include <string>
#include <unordered_map>

static const std::string TEXTURE_PATH = "assets/textures/";
static const std::string SPRITE_PATH = "assets/sprites/";
static const std::string FONT_PATH = "assets/fonts/";
static const std::string WORLDS_PATH = "assets/worlds/";

static const std::string DEFAULT_FONT = "UbuntuMono-Bold.ttf";

static const std::unordered_map<std::string, std::string> FONT_DEFAULTS_MAP = {
    {"default", DEFAULT_FONT},
};

const std::string& AssetCache::defaultSpritePath() {
	return SPRITE_PATH;
}

const std::string& AssetCache::defaultWorldsPath() {
	return WORLDS_PATH;
}

std::shared_ptr<Texture> AssetCache::texture(const std::string& name) {
	LINFO("Try to load: {} texture", name);
	if (auto found = _textures.find(name); found != _textures.end()) {
		LINFO("Texture {} found in  cache", name);
		return found->second;
	}

	auto texture = std::make_shared<Texture>(name);
	if (texture->loadFromFile(TEXTURE_PATH + name)) {
		_textures.emplace(name, texture);
		return texture;
	}

	LINFO("Texture {} not found", name);
	return nullptr;
}

std::shared_ptr<Sprite> AssetCache::sprite(const std::string& name) {
	LINFO("Try to load: {} sprite", name);
	if (auto found = _sprites.find(name); found != _sprites.end()) {
		LINFO("Sprite {} found in cache", name);
		return found->second;
	}

	auto sprite = std::make_shared<Sprite>(name);
	if (sprite->loadFromFile(SPRITE_PATH + name, *this)) {
		_sprites.emplace(name, sprite);
		LINFO("Sprite {} loaded", name);
		return sprite;
	}

	LINFO("Sprite {} not found in cache", name);
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

std::shared_ptr<Scene> AssetCache::scene(const std::string& name) {
	std::shared_ptr<Scene> scene;

#define CHECK(T, NAME)                                \
	if (name == NAME) {                               \
		if (_scenes.contains(name)) {                 \
			LINFO("Load scene from cache: {}", name); \
			scene = _scenes[name];                    \
		} else {                                      \
			scene = std::make_shared<T>(_context);    \
			_scenes.emplace(name, scene);             \
			LINFO("Create scene: {}", name);          \
		}                                             \
		return scene;                                 \
	}

	CHECK(MainMenu, "main_menu");
	CHECK(DevMenu, "dev_menu");
	CHECK(SpriteEditor, "sprite_editor");
	CHECK(SpritesView, "sprites_view");
	CHECK(WorldEditor, "world_editor");
	CHECK(DevMenu, "dev_menu");
	CHECK(TestScene, "test_scene");
#undef CHECK

	LERR("Scene not found: {}", name);

	return scene;
}

std::vector<std::string> AssetCache::sprites() const {
	std::vector<std::string> result;
	for (const auto& entry : std::filesystem::directory_iterator(SPRITE_PATH)) {
		if (entry.is_regular_file()) {
			result.push_back(entry.path().filename().string());
		}
	}

	return result;
}

std::vector<std::string> AssetCache::worlds() const {
	std::vector<std::string> result;
	for (const auto& entry : std::filesystem::directory_iterator(WORLDS_PATH)) {
		if (entry.is_directory()) {
			result.push_back(entry.path().filename().string());
		}
	}

	return result;
}

std::shared_ptr<Font> AssetCache::font(const std::string& name) {
	if (auto found = _fonts.find(name); found != _fonts.end()) {
		LINFO("Load font from cache: {}", name);

		return found->second;
	}

	auto font = std::make_shared<Font>(name);
	if (font->loadFromFile(FONT_PATH + name)) {
		LINFO("Load font to cache: {}", name);
		_fonts.emplace(name, font);

		return font;
	}

	if (auto found = FONT_DEFAULTS_MAP.find(name); found != FONT_DEFAULTS_MAP.end()) {
		if (!font->loadFromFile(FONT_PATH + found->second)) {
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

World AssetCache::world(const std::string& name) {
	// std::ifstream f(WORLD_PATH + name);
	// IArchive ar(f);
	World w;
	// ar >> w;

	return w;
}

void AssetCache::world(const World& world, const std::string& name) {
	// std::ofstream f(WORLD_PATH + name);
	// OArchive ar(f);
	// ar << world;
}