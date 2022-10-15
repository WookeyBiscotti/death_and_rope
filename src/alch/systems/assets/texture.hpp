#pragma once

#include "asset.hpp"

#include <SFML/Graphics/Texture.hpp>

namespace al {

class Texture: public Asset {
  public:
	explicit Texture(std::string name): Asset(std::move(name)){};

	bool loadFromFile(const std::string& path);

	sf::Texture& sf() { return _texture; }

  private:
	sf::Texture _texture;
};

}
