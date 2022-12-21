#include "ui_system.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/window/events.hpp"
#include "alch/systems/window/window.hpp"
#include "ui_element.hpp"
#include "ui_events.hpp"

using namespace al;

class UnchangeableLayout: public UIElement {
  public:
	UnchangeableLayout(Context& context, WeakPtr<UIElement> parent): UIElement(context, parent) {
		_layout = UIElement::FREE;
	}

	bool isPointInside(const Vector2f& p) const override {
		for (const auto& c : _childs) {
			if (c->isPointInside(p)) {
				return true;
			}
		}

		return false;
	}
};

UIElement* UISystem::getElementUnderPoint(UIElement* el, Vector2f p) {
	if (el->isGlobalPointIn(p)) {
		for (auto it = el->_childs.rbegin(); it != el->_childs.rend(); ++it) {
			const auto& c = *it;
			if (c->isGlobalPointIn(p)) {
				if (auto ret = getElementUnderPoint(c.get(), p)) {
					return ret;
				}
			}
		}
	} else {
		return nullptr;
	}

	return el->eventable() ? el : nullptr;
}

UIElement* findWidgetUnderPoint(UIElement& root, const Vector2f& p) {
	UIElement* last = nullptr;
	if (root.isPointInside(p)) {
		last = &root;
		for (const auto& c : root.childs()) {
			if (c->isPointInside(p)) {
				last = c.get();
				last = findWidgetUnderPoint(*c.get(), p);
			}
		}
	}

	return last;
}

UISystem::UISystem(Context& context): System(context) {
	_root = SharedPtr<UIElement>::make(_context, nullptr);
	_userRoot = _root->create<UIElement>();
	_freeLayout = _root->create<UnchangeableLayout>();
	_lastHovered = _userRoot;
	const auto windowSize = context.systemRef<Window>().window().getSize();
	_root->size(Vector2f(windowSize.x, windowSize.y));
	_userRoot->size(Vector2f(windowSize.x, windowSize.y));
	_freeLayout->size(Vector2f(windowSize.x, windowSize.y));

	subscribe<WindowUIEvent>([this](const WindowUIEvent& e) {
		const auto& eType = e.general.event.type;
		if (eType != sf::Event::Closed && eType != sf::Event::LostFocus && eType != sf::Event::GainedFocus &&
		    eType != sf::Event::Resized) {
			if (eType == sf::Event::MouseMoved) {
				if (_lastDraged.empty()) {
					auto w = findWidgetUnderPoint(*_root,
					    Vector2f(e.general.event.mouseMove.x, e.general.event.mouseMove.y));

					if (_lastHovered != w) {
						if (_lastHovered) {
							_lastHovered->onUnhovered({UIUnhovered{e.general.event.mouseMove}});
						}
						_lastHovered = w;
						if (_lastHovered) {
							_lastHovered->onHovered({UIHovered{e.general.event.mouseMove}});
						}
					} else {
						if (_lastHovered) {
							_lastHovered->onMouseMove({UIMouseMove{e.general.event.mouseMove}});
						}
					}
				} else {
					for (auto& [_, w] : _lastDraged) {
						if (w) {
							w->onDrag({e.general.event.mouseMove});
						}
					}
				}
			} else if (eType == sf::Event::MouseButtonPressed) {
				auto w =
				    findWidgetUnderPoint(*_root, Vector2f(e.general.event.mouseMove.x, e.general.event.mouseButton.y));
				if (w) {
					w->onDragStart(UIMouseDragStart{e.general.event.mouseButton});
					w->onPressed(UIMouseButtonPressed{e.general.event.mouseButton});
					_lastDraged[e.general.event.mouseButton.button] = w;
				}
			} else if (eType == sf::Event::MouseButtonReleased) {
				if (auto f = _lastDraged.find(e.general.event.mouseButton.button); f != _lastDraged.end()) {
					f->second->onReleased(UIMouseButtonReleased{e.general.event.mouseButton});
					f->second->onDragStop(UIMouseDragStop{e.general.event.mouseButton});
					_lastDraged.erase(f);
				}
			} else if (eType == sf::Event::MouseWheelScrolled) {
				auto w = _root->onMouseWheel(UIMouseWheel{e.general.event.mouseWheelScroll});
			}
		}
		if (eType == sf::Event::Resized) {
			_root->size(Vector2f(e.general.event.size.width, e.general.event.size.height));
			_freeLayout->size(Vector2f(e.general.event.size.width, e.general.event.size.height));
			_userRoot->size(Vector2f(e.general.event.size.width, e.general.event.size.height));
		}
		// if (eType == sf::Event::MouseLeft) {
		// 	if (_lastDraged) {
		// 		_lastHovered->onDragStop({});
		// 	}
		// }
	});
}

UIElement* UISystem::root() {
	return _userRoot;
}

UISystem::~UISystem() {
}

void UISystem::render() {
	auto& w = _context.systemRef<Window>();
	auto currentView = w.window().getView();
	w.window().setView(w.window().getDefaultView());
	_root->draw(w.window());
	w.window().setView(currentView);
}

UIElement* UISystem::popout() {
	return _freeLayout;
}
