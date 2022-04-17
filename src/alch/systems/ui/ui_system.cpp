#include "ui_system.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/window/events.hpp"
#include "alch/systems/window/window.hpp"
#include "ui_element.hpp"
#include "ui_events.hpp"

UIElement* UISystem::getElementUnderPoint(UIElement* el, Vector2f p) {
	if (el->isGlobalPointIn(p)) {
		for (const auto& c : el->_childs) {
			if (c->isGlobalPointIn(p)) {
				return getElementUnderPoint(c.get(), p);
			}
		}
	} else {
		return nullptr;
	}

	return el;
}

UISystem::UISystem(Context& context): Receiver(context.systemRef<Broker>()), _context(context) {
	_root = std::make_unique<UIElement>(nullptr, *this);
	_lastHovered = _root.get();
	const auto windowSize = context.systemRef<Window>().window().getSize();
	_root->size(Vector2f(windowSize.x, windowSize.y));

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
		}
		if (eType == sf::Event::MouseLeft) {
			if (_lastDraged) {
				_lastHovered->onDragStop({});
			}
		}
	});
}

UIElement* UISystem::root() {
	return _root.get();
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
