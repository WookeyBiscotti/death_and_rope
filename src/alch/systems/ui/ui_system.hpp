#pragma once

#include "alch/common/vector2.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/broker/receiver.hpp"
//
#include <SFML/Window/Mouse.hpp>
#include <memory>

namespace al {

class Context;
class UIElement;

class UISystem final: public Receiver, public System {
  public:
	explicit UISystem(Context& context);
	~UISystem();

	UIElement* root();
	UIElement* popout();

	void render();

	static UIElement* getElementUnderPoint(UIElement* el, Vector2f p);

	void remove(UIElement* el) {
		if (_lastHovered == el) {
			_lastHovered = nullptr;
		}
		for (auto& [k, v] : _lastDraged) {
			if (v == el) {
				_lastDraged.erase(k);
			}
		}
	}

	UIElement* lastHovered() const { return _lastHovered; }
	void lastHovered(UIElement* lastHovered) { _lastHovered = lastHovered; }

	UIElement* lastDraged(sf::Mouse::Button b) const {
		if (auto found = _lastDraged.find(b); found != _lastDraged.end()) {
			return found->second;
		}

		return nullptr;
	}

	void lastDraged(sf::Mouse::Button b, UIElement* lastDraged) { _lastDraged[b] = lastDraged; }

  private:
	std::unique_ptr<UIElement> _root;
	UIElement* _freeLayout;
	UIElement* _userRoot;
	Context& _context;

	UIElement* _lastHovered{};
	std::unordered_map<sf::Mouse::Button, UIElement*> _lastDraged;
};

}
