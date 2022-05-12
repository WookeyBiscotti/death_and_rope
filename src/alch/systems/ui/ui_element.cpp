#include "ui_element.hpp"

#include "alch/engine/context.hpp"
#include "ui_system.hpp"

UIElement::UIElement(UIElement* parent, Context& context): _parent(parent), _context(context) {
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
	if (_layout == FREE) {
		// for (auto& c : _childs) {
		// 	c->onResize();
		// }
	} else if (_layout == HORIZONTAL) {
		if (_childs.size() == 1) {
			if (_childs.front()->resizeable()) {
				_childs.front()->size(_size, true);
			}
			return;
		}
		auto w = _size.x;
		int count = 0;
		for (auto& c : _childs) {
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
		for (auto& c : _childs) {
			if (!c->resizeable()) {
				c->size(Vector2f(c->size().x, _size.y), true);
			} else {
				c->size(Vector2f(dw, _size.y), true);
			}
			c->position(Vector2f(startPos, 0));
			startPos += c->size().x;
		}
	} else if (_layout == VERICAL) {
		if (_childs.size() == 1) {
			if (_childs.front()->resizeable()) {
				_childs.front()->size(_size, true);
			}
			return;
		}
		auto h = _size.y;
		int count = 0;
		for (auto& c : _childs) {
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
		for (auto& c : _childs) {
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
	if (eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventInside(e)) {
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
	if (eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventInside(e)) {
				return c->onPressed(e);
			}
		}
	}

	return nullptr;
}

UIElement* UIElement::onReleased(const UIMouseButtonReleased& e) {
	if (eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventInside(e)) {
				return c->onReleased(e);
			}
		}
	}

	return nullptr;
}

UIElement* UIElement::onDragStart(const UIMouseDragStart& e) {
	if (eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventInside(e)) {
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
	if (eventInside(e)) {
		for (auto& c : _childs) {
			if (c->eventInside(e)) {
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
