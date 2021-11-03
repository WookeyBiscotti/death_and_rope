#pragma once

#include "asset.hpp"
#include "rect.hpp"
#include "texture.hpp"
#include "vector2.hpp"
//
#include <SFML/Graphics.hpp>
//
#include <memory>

class Sprite: public Asset {
	friend class SpriteEditor;

  public:
	Sprite(std::string name): Asset(std::move(name)){};

	bool loadFromFile(const std::string& path, AssetCache& cache);
	bool saveToFile(const std::string& path);

	void draw(sf::RenderTarget& target, const Vector2f& p);

	sf::Sprite& sf() { return _sprite; }

  private:
	std::shared_ptr<Texture> _texture;
	Recti _textureRect;

	sf::Sprite _sprite;
};