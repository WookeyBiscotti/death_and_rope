#pragma once

#include <systems/assets/font.hpp>
#include <systems/broker/receiver.hpp>
//
#include <SFML/Graphics/Text.hpp>
//
#include <memory>

class Context;

class DebugSystem: private Receiver {
  public:
	explicit DebugSystem(Context& context);

  private:
	Context& _context;

	std::shared_ptr<Font> _font;

	sf::Text _text;
	std::size_t _frameCounter = 60;
	float _lastFps{};

	bool _isLogShown{};
};