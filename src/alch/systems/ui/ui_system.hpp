#pragma once

#include "alch/common/vector2.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/broker/receiver.hpp"
//
#include <memory>

class Context;
class UIElement;

class UISystem final: public Receiver, public System {
  public:
	explicit UISystem(Context& context);
	~UISystem();

	UIElement* root();

	void render();

	static UIElement* getElementUnderPoint(UIElement* el, Vector2f p);

	void remove(UIElement* el) {
		if (_lastHovered == el) {
			_lastHovered = nullptr;
		}
	};

  private:
	std::unique_ptr<UIElement> _root;
	Context& _context;

	UIElement* _lastHovered{};
	UIElement* _lastDraged{};
};
