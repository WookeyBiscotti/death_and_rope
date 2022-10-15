#include "ui_element.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

#include <vector>

using namespace al;

UIElement::UIElement(UIElement* parent, Context& context):
    Sender(context.systemRef<Broker>()), Receiver(context.systemRef<Broker>()), _parent(parent), _context(context) {
}

UIElement::~UIElement() {
	system().remove(this);
}

UISystem& UIElement::system() const {
	return _context.systemRef<UISystem>();
}

void UIElement::onMove() {
	for (auto& c : _childs) {
		c->onMove();
	}
}

void UIElement::onResize() {
	std::vector<UIElement*> childs;
	for (auto& c : _childs) {
		if (c) {
			childs.push_back(c.get());
		}
	};
	if (childs.empty()) {
		return;
	}

	if (_layout == FREE) {
		// for (auto& c : _childs) {
		// 	c->onResize();
		// }
	} else if (_layout == HORIZONTAL) {
		if (childs.size() == 1) {
			if (childs.front()->resizeable()) {
				childs.front()->size(_size, true);
			} else {
				childs.front()->size({childs.front()->size().x, size().y}, true);
			}
			return;
		}
		auto w = _size.x;
		int count = 0;
		for (auto& c : childs) {
			if (!c->resizeable()) {
				w -= c->size().x;
			} else {
				++count;
			}
		}
		if (count == 0) {
			return;
		}
		const auto dw = w / count;
		float startPos = 0.0f;
		for (auto& c : childs) {
			if (!c->resizeable()) {
				c->size(Vector2f(c->size().x, _size.y), true);
			} else {
				c->size(Vector2f(dw, _size.y), true);
			}
			c->position(Vector2f(startPos, 0));
			startPos += c->size().x;
		}
	} else if (_layout == VERICAL) {
		if (childs.size() == 1) {
			if (childs.front()->resizeable()) {
				childs.front()->size(_size, true);
			} else {
				childs.front()->size({size().x, childs.front()->size().y}, true);
			}
			return;
		}
		auto h = _size.y;
		int count = 0;
		for (auto& c : childs) {
			if (!c->resizeable()) {
				h -= c->size().y;
			} else {
				++count;
			}
		}
		if (count == 0) {
			return;
		}
		const auto dh = h / count;
		float startPos = 0.0f;
		for (auto& c : childs) {
			if (!c->resizeable()) {
				c->size(Vector2f(_size.x, c->size().y), true);
			} else {
				c->size(Vector2f(_size.x, dh), true);
			}
			c->position(Vector2f(0, startPos));
			startPos += c->size().y;
		}
	}
}

void UIElement::add(std::unique_ptr<UIElement> element) {
	auto e = element.get();
	element->parent(this);
	_childs.push_back(std::move(element));
	e->onMove();

	if (_layout != FREE) {
		onResize();
	}
}

void UIElement::add(UIElement* element) {
	add(std::unique_ptr<UIElement>(element));
}

UIElement* UIElement::onMouseMove(const UIMouseMove& e) {
	if (eventable() && eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventable() && c->eventInside(e)) {
				if (system().lastHovered() == c.get()) {

					return c->onMouseMove({e.event});
				} else {
					auto hovered = c->onHovered({e.event});

					if (system().lastHovered() && system().lastHovered() != hovered) {
						system().lastHovered()->onUnhovered({e.event});
						system().lastHovered(hovered);
					}

					return hovered;
				}
			}
		}

		return this;
	}

	return nullptr;
}

UIElement* UIElement::onHovered(const UIHovered& e) {
	return UIElement::onMouseMove({e.event});
}

UIElement* UIElement::onUnhovered(const UIUnhovered& e) {
	return nullptr;
}

UIElement* UIElement::onPressed(const UIMouseButtonPressed& e) {
	if (eventable() && eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventable() && c->eventInside(e)) {
				return c->onPressed(e);
			}
		}
	}

	return nullptr;
}

UIElement* UIElement::onReleased(const UIMouseButtonReleased& e) {
	if (eventable() && eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventable() && c->eventInside(e)) {
				return c->onReleased(e);
			}
		}
	}

	return nullptr;
}

UIElement* UIElement::onDragStart(const UIMouseDragStart& e) {
	if (eventable() && eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventable() && c->eventInside(e)) {
				return c->onDragStart(e);
			}
		}
	}

	return nullptr;
}

UIElement* UIElement::onDrag(const UIMouseDrag& e) {
	return nullptr;
}

UIElement* UIElement::onDragStop(const UIMouseDragStop& e) {
	return nullptr;
}

UIElement* UIElement::onMouseWheel(const UIMouseWheel& e) {
	if (eventable() && eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventable() && c->eventInside(e)) {
				auto wg = c->onMouseWheel(e);
				if (wg) {
					return wg;
				}
			}
		}

		for (auto& c : _childs) {
			auto wg = c->onMouseWheel(e);
			if (wg) {
				return wg;
			}
		}
	}

	return nullptr;
}

void UIElement::parent(UIElement* parent) {
	_parent = parent;
	onResize();
	onMove();
}

void UIElement::position(Vector2f position) {
	_position = position;
	onMove();
}

void UIElement::size(Vector2f size, bool noParentCallback) {
	if (size == _size) {
		return;
	}
	_size = size;
	onResize();
	if (!noParentCallback && _parent) {
		_parent->onResize();
	}
}

void UIElement::resizeable(bool resizeable) {
	_resizeable = resizeable;
	if (_parent) {
		_parent->onResize();
	}
}

void UIElement::remove(UIElement* element) {
	for (auto it = _childs.begin(); it != _childs.end(); ++it) {
		if (it->get() == element) {
			_childs.erase(it);
			onResize();
			break;
		}
	}
}

void UIElement::enabled(bool enabled) {
	if (_enabled == enabled && _parent) {
		_parent->onResize();
	}

	_enabled = enabled;
}

void UIElement::draw(sf::RenderTarget& target) {
	for (auto& c : _childs) {
		if (c->enabled()) {
			c->draw(target);
		}
	}
};
