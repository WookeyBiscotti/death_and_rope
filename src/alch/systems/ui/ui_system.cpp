#include "ui_system.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/window/events.hpp"
#include "alch/systems/window/window.hpp"
#include "ui_element.hpp"
#include "ui_events.hpp"

class UnchangeableLayout: public UIElement {
  public:
	UnchangeableLayout(UIElement* parent, Context& context): UIElement(parent, context) { _layout = UIElement::FREE; }

	void layout(Layout) override {}
};

UIElement* UISystem::getElementUnderPoint(UIElement* el, Vector2f p) {
	if(!el->_debug.empty()) {
		int a = 0;
	}
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

UISystem::UISystem(Context& context): Receiver(context.systemRef<Broker>()), _context(context) {
	_root = std::make_unique<UIElement>(nullptr, _context);
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
				if (_lastDraged) {
					_lastDraged->onDrag(UIMouseDrag{e.general.event.mouseMove});
				}
				auto el = getElementUnderPoint(
				    _root.get(), Vector2f(e.general.event.mouseMove.x, e.general.event.mouseMove.y));
				if (el) {
					if (_lastHovered != el) {
						if (_lastHovered) {
							_lastHovered->onUnhovered(UIUnhovered{e.general.event.mouseMove});
						}
						_lastHovered = el;
					}
					e.eventCaptured = el->onHovered(UIHovered{e.general.event.mouseMove});
				}
			} else if (eType == sf::Event::MouseButtonPressed) {
				if (_lastDraged) {
					_lastDraged->onDragStop({});
				}
				auto el = getElementUnderPoint(
				    _root.get(), Vector2f(e.general.event.mouseButton.x, e.general.event.mouseButton.y));
				if (el) {
					e.eventCaptured = el->onPressed(UIMouseButtonPressed{e.general.event.mouseButton});
					el->onDragStart(UIMouseDragStart{e.general.event.mouseButton});
					_lastDraged = el;
				}
			} else if (eType == sf::Event::MouseButtonReleased) {
				if (_lastDraged) {
					_lastDraged->onDragStop({});
					_lastDraged = {};
				}
				auto el = getElementUnderPoint(
				    _root.get(), Vector2f(e.general.event.mouseButton.x, e.general.event.mouseButton.y));
				if (el) {
					e.eventCaptured = el->onReleased(UIMouseButtonReleased{e.general.event.mouseButton});
				}
			}
		}
		if (eType == sf::Event::Resized) {
			_root->size(Vector2f(e.general.event.size.width, e.general.event.size.height));
			_freeLayout->size(Vector2f(e.general.event.size.width, e.general.event.size.height));
			_userRoot->size(Vector2f(e.general.event.size.width, e.general.event.size.height));
		}
		if (eType == sf::Event::MouseLeft) {
			if (_lastDraged) {
				_lastHovered->onDragStop({});
			}
		}
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
