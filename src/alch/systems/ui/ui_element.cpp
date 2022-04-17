#include "ui_element.hpp"

#include "ui_system.hpp"

UIElement::~UIElement() {
	_system.remove(this);
}

void UIElement::onMove() {
	for (auto& c : _childs) {
		c->onMove();
	}
}

void UIElement::onResize() {
	if (_layout == FREE) {
	} else if (_layout == HORIZONTAL) {
		if (_childs.size() == 1) {
			if (_childs.front()->resizeable()) {
				_childs.front()->size(_size);
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
				c->size(Vector2f(c->size().x, _size.y));
			} else {
				c->size(Vector2f(dw, _size.y));
			}
			c->position(Vector2f(startPos, 0));
			startPos += c->size().x;
		}
	} else if (_layout == VERICAL) {
		if (_childs.size() == 1) {
			if (_childs.front()->resizeable()) {
				_childs.front()->size(_size);
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
				c->size(Vector2f(_size.x, c->size().y));
			} else {
				c->size(Vector2f(_size.x, dh));
			}
			c->position(Vector2f(0, startPos));
			startPos += c->size().y;
		}
	}
}

void UIElement::add(std::unique_ptr<UIElement> element) {
	_childs.push_back(std::move(element));

	if (_layout != FREE) {
		onResize();
	}
}

void UIElement::add(UIElement* element) {
	add(std::unique_ptr<UIElement>(element));
}
