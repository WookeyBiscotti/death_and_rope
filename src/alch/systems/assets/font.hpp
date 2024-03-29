#pragma once

#include "asset.hpp"
//
#include <SFML/Graphics/Font.hpp>

namespace al {

class Font: public Asset {
  public:
	explicit Font(std::string name): Asset(std::move(name)){};

	bool loadFromFile(const std::string& path) { return _font.loadFromFile(path); }

	sf::Font& sf() { return _font; }

  private:
	sf::Font _font;
};

}
