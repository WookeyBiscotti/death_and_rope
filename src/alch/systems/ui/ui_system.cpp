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
	if (!el->_debug.empty()) {
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
	const auto windowSize = context.systemRef<WindowSystem>().window().getSize();
	_root->size(Vector2f(windowSize.x, windowSize.y));
	_userRoot->size(Vector2f(windowSize.x, windowSize.y));
	_freeLayout->size(Vector2f(windowSize.x, windowSize.y));

	subscribe<WindowUIEvent>([this](const WindowUIEvent& e) {
		const auto& eType = e.general.event.type;
		if (eType != sf::Event::Closed && eType != sf::Event::LostFocus && eType != sf::Event::GainedFocus &&
		    eType != sf::Event::Resized) {
			if (eType == sf::Event::MouseMoved) {
				_freeLayout->onMouseMove({UIMouseMove{e.general.event.motion}}) ||
				    _userRoot->onMouseMove({UIMouseMove{e.general.event.motion}});
				for (auto& [_, w] : _lastDraged) {
					if (w) {
						w->onDrag({e.general.event.motion});
					}
				}
			} else if (eType == sf::Event::MouseButtonPressed) {
				_freeLayout->onPressed(UIMouseButtonPressed{e.general.event.button}) ||
				    _userRoot->onPressed(UIMouseButtonPressed{e.general.event.button});
				auto wg = _freeLayout->onDragStart(UIMouseDragStart{e.general.event.button});
				if (!wg) {
					wg = _userRoot->onDragStart({UIMouseDragStart{e.general.event.button}});
				}
				if (wg) {
					_lastDraged[e.general.event.button.button] = wg;
				}
			} else if (eType == sf::Event::MouseButtonReleased) {
				_freeLayout->onReleased(UIMouseButtonReleased{e.general.event.button}) ||
				    _userRoot->onReleased(UIMouseButtonReleased{e.general.event.button});
				auto wg = _lastDraged[e.general.event.button.button];
				if (wg) {
					wg->onDragStop({e.general.event.button});
				}
				_lastDraged.erase(e.general.event.button.button);
			} else if (eType == sf::Event::MouseWheelScrolled) {
				_freeLayout->onMouseWheel(UIMouseWheel{e.general.event.wheel}) ||
				    _userRoot->onMouseWheel(UIMouseWheel{e.general.event.wheel});
			}
		}
		if (eType == SDL_WINDOWEVENT_RESIZED) {
			_root->size(Vector2f(e.general.event.window.data1, e.general.event.window.data2));
			_freeLayout->size(Vector2f(e.general.event.window.data1, e.general.event.window.data2));
			_userRoot->size(Vector2f(e.general.event.window.data1, e.general.event.window.data2));
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
