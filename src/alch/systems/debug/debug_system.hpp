#pragma once

#include "alch/common/smart_ptr.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/assets/font.hpp"

//
#include <SFML/Graphics/Text.hpp>
//
#include <array>
#include <deque>
#include <memory>

namespace al {

class Context;
class Box2dDebugDraw;

class DebugSystem: public System {
  public:
	explicit DebugSystem(Context& context);

  private:
	void dumpHistoryToFilesystem();
	void loadHistoryFromFilesystem();

  private:
	SharedPtr<Font> _font;

	sf::Text _text;
	std::size_t _frameCounter = 60;
	float _lastFps{};

	bool _isLogShown{};

	std::shared_ptr<Box2dDebugDraw> _debugBox2dDraw;
	std::deque<std::string> _commandsHistory;
	int _commandsHistoryPos = -1;
	std::array<char, 2048> _buffer;
};

} // namespace al
