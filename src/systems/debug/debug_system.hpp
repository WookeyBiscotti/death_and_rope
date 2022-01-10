#pragma once

#include <engine/system.hpp>
#include <systems/assets/font.hpp>
#include <systems/broker/receiver.hpp>
//
#include <SFML/Graphics/Text.hpp>
//
#include <array>
#include <deque>
#include <memory>

class Context;
class Box2dDebugDraw;

class DebugSystem: private Receiver, public System {
  public:
	explicit DebugSystem(Context& context);

  private:
	void dumpHistoryToFilesystem();
	void loadHistoryFromFilesystem();

  private:
	Context& _context;

	std::shared_ptr<Font> _font;

	sf::Text _text;
	std::size_t _frameCounter = 60;
	float _lastFps{};

	bool _isLogShown{};

	std::shared_ptr<Box2dDebugDraw> _debugBox2dDraw;
	std::deque<std::string> _commandsHistory;
	int _commandsHistoryPos = -1;
	std::array<char, 2048> _buffer;
};