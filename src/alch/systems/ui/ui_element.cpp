#include "ui_element.hpp"

#include "alch/common/containers/inline_vector.hpp"
#include "alch/common/test_framework.hpp"
#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

#include <vector>

using namespace al;

UIElement::UIElement(Context& context, WeakPtr<UIElement> parent):
    Transmitter(context.systemRef<Broker>()), _parent(parent), _context(context) {
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

void UIElement::updateChildsSize() {
	if (_layout == FREE) {
		return;
	}

	if (_childs.empty()) {
		return;
	}

	auto stacker = [this](UIUnit Vector2<UIUnit>::*C) {
		if (_childs.size() == 1) {
			_childs.front()->size(_size);
			return;
		}

		const auto dw = _size.*C / _childs.size();
		auto idx = 0;
		for (const auto& c : _childs) {
			Vector2<UIUnit> pos{};
			pos.*C = idx * dw;
			c->position(pos);

			Vector2<UIUnit> size(_size);
			size.*C = dw;
			c->size(size);
		}
	};

	if (_layout == UIElement::HORIZONTAL) {
		stacker(&Vector2<UIUnit>::x);
	} else {
		stacker(&Vector2<UIUnit>::y);
	}
}

void UIElement::onResize() {
	// TODO:use small vector
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
			// if (childs.front()->resizeable()) {
			// 	childs.front()->size(_size, true);
			// } else {
			// 	childs.front()->size({childs.front()->size().x, size().y}, true);
			// }
			// return;
		}
		auto w = _size.x;
		int count = 0;
		for (auto& c : childs) {
			// if (!c->resizeable()) {
			// 	w -= c->size().x;
			// } else {
			// 	++count;
			// }
		}
		if (count == 0) {
			return;
		}
		const auto dw = w / count;
		float startPos = 0.0f;
		for (auto& c : childs) {
			// if (!c->resizeable()) {
			// 	c->size(Vector2f(c->size().x, _size.y), true);
			// } else {
			// 	c->size(Vector2f(dw, _size.y), true);
			// }
			c->position(Vector2f(startPos, 0));
			startPos += c->size().x;
		}
	} else if (_layout == VERICAL) {
		if (childs.size() == 1) {
			// if (childs.front()->resizeable()) {
			// 	childs.front()->size(_size, true);
			// } else {
			// 	childs.front()->size({size().x, childs.front()->size().y}, true);
			// }
			// return;
		}
		auto h = _size.y;
		int count = 0;
		for (auto& c : childs) {
			// if (!c->resizeable()) {
			// 	h -= c->size().y;
			// } else {
			// 	++count;
			// }
		}
		if (count == 0) {
			return;
		}
		const auto dh = h / count;
		float startPos = 0.0f;
		for (auto& c : childs) {
			// if (!c->resizeable()) {
			// 	c->size(Vector2f(_size.x, c->size().y), true);
			// } else {
			// 	c->size(Vector2f(_size.x, dh), true);
			// }
			// c->position(Vector2f(0, startPos));
			startPos += c->size().y;
		}
	}
}

void UIElement::add(SharedPtr<UIElement> element) {
	auto e = element.get();
	element->parent(sharedFromThis());
	_childs.push_back(std::move(element));
	e->onMove();

	updateChildsSize();
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

void UIElement::parent(WeakPtr<UIElement> parent) {
	_parent = parent;
	onResize();
	onMove();
}

Vector2<UIUnit> UIElement::position() const {
	return _position;
}

void UIElement::position(Vector2<UIUnit> position) {
	_position = position;
	onMove();
}

void UIElement::updatePositionPart() {
}

const Vector2<Opt<float>>& UIElement::positionPart() const {
	return _positionPart;
}

void UIElement::positionPart(const Vector2<Opt<float>>& position) {
}

void UIElement::size(Vector2<UIUnit> size, bool noParentCallback) {
	if (size == _size) {
		return;
	}
	_size = size;
	onResize();
	auto p = _parent.lock();
	if (!noParentCallback && p) {
		p->onResize();
	}
}

void UIElement::remove(UIElement* element) {
	for (auto it = _childs.begin(); it != _childs.end(); ++it) {
		if (it->get() == element) {
			_childs.erase(it);
			updateChildsSize();
			break;
		}
	}
}
void UIElement::removeAll() {
	_childs.clear();
}

void UIElement::enabled(bool enabled) {
	auto p = _parent.lock();
	if (_enabled == enabled && p) {
		p->onResize();
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
