#pragma once

#include "alch/common/smart_ptr.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/system.hpp"
#include "ui_style.hpp"

//
#include <SFML/Window/Mouse.hpp>

namespace al {

class Context;
class UIElement;

class UISystem final: public System {
  public:
	explicit UISystem(Context& context);
	~UISystem();

	UIElement* root();
	UIElement* popout();

	void render();

	static UIElement* getElementUnderPoint(UIElement* el, Vector2f p);

	void remove(UIElement* el);

	UIElement* lastHovered() const { return _lastHovered; }
	void lastHovered(UIElement* lastHovered) { _lastHovered = lastHovered; }

	UIElement* lastDraged(sf::Mouse::Button b) const;
	void lastDraged(sf::Mouse::Button b, UIElement* lastDraged) { _lastDraged[b] = lastDraged; }

  private:
	SharedPtr<UIElement> _root;
	UIElement* _freeLayout;
	UIElement* _userRoot;

	UIElement* _lastHovered{};
	std::unordered_map<sf::Mouse::Button, UIElement*> _lastDraged;
};

inline UIElement* UISystem::lastDraged(sf::Mouse::Button b) const {
	if (auto found = _lastDraged.find(b); found != _lastDraged.end()) {
		return found->second;
	}

	return nullptr;
}

inline void UISystem::remove(UIElement* el) {
	if (_lastHovered == el) {
		_lastHovered = nullptr;
	}
	for (auto& [k, v] : _lastDraged) {
		if (v == el) {
			_lastDraged.erase(k);
		}
	}
}

} // namespace al
