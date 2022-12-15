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
	constexpr UIUnit EPSILON = 0.00001;
	if (_layout == FREE) {
		return;
	}

	if (_childs.empty()) {
		return;
	}

	auto stacker = [this]<UIUnit Vector2<UIUnit>::*C>(int gravityDir /*[-1,1]*/) {
		for (auto& c : _childs) {
			c->_size.*C = 0;
		}

		auto w = _size.*C;
		auto count = _childs.size();
		auto dw = w / count;
		for (const auto& c : _childs) {
			if (c->_minSize.*C > dw) {
				w -= c->_minSize.*C;
				--count;
				if (count != 0) {
					dw = w / count;
				}
			}
		}
		if (w >= EPSILON) {
			for (const auto& c : _childs) {
				w -= (c->_maxSize.*C - c->_minSize.*C);
				if (w < EPSILON) {
					break;
				}
			}
			if (w > EPSILON) {
				// места много и мы не в состоянии его заполнить
				w = 0;
				for (const auto& c : _childs) {
					c->_size.*C = c->_maxSize.*C;
					w += c->_size.*C;
				}

				dw = w / _childs.size();

				w = 0;
				for (const auto& c : _childs) {
					Vector2<UIUnit> pos{};
					pos.*C = w + dw * (1 + gravityDir) / 2;
					c->position(pos);
					w += c->_size.*C + dw * (1 - gravityDir) / 2;
				}

			} else
				// хватает места и мы можем полностью его заполнить
				while (w < EPSILON) {
					w = _size.*C;

					for (auto& c : _childs) {
						c->_size.*C = c->_minSize.*C;
						w -= c->_minSize.*C;
					}
					if (w <= EPSILON) {
						break;
					}

					UIUnit minSize{std::numeric_limits<UIUnit>::max()};
					InlinedVector<UIElement*, 16> elms;
					for (auto& c : _childs) {
						elms.push_back(c.get());
						if (c->_size.*C < minSize) {
							minSize = c->_size.*C;
						}
					}

					while (!elms.empty() && w >= EPSILON) {
						dw = w / elms.size();

						UIUnit newMinSize{std::numeric_limits<UIUnit>::max()};
						for (auto& e : elms) {
							if (e->_maxSize.*C <= minSize + dw) {
								w -= e->_maxSize.*C - e->_size.*C;
								e->_size.*C = e->_maxSize.*C;

								std::swap(e, elms.back());
								elms.pop_back();

								break;
							} else if (e->_size.*C < dw + minSize) {
								w -= (dw + minSize - e->_size.*C);
								e->_size.*C = dw + minSize;
								if (e->_size.*C < newMinSize) {
									newMinSize = e->_size.*C;
								}
							}

							if (&e == &elms.back()) {
								minSize = newMinSize;
							}
						}
					}

					break;
				}
		} else {
			// все виджеты не помещаются в выделенное место
			LWARN("We cant put all child widget in parent winget: not enough space");
			for (const auto& c : _childs) {
				c->_size.*C = c->_minSize.*C;
			}
		}

		w = 0;
		for (const auto& c : _childs) {
			Vector2<UIUnit> pos{};
			pos.*C = w;
			c->position(pos);
			w += c->_size.*C;
		}
	};

	auto gravityToDir = [](auto g) {
		auto val = static_cast<int>(g);
		if (val == 0) {
			return 0;
		} else if (val == 1) {
			return -1;
		} else {
			return 1;
		}
	};

	auto placeOrthogonal = [this]<UIUnit Vector2<UIUnit>::*C>(int gravityDir /*[-1,1]*/) {
		for (const auto& c : _childs) {
			if (_size.*C >= c->_minSize.*C && _size.*C <= c->_maxSize.*C) {
				c->_size.*C = _size.*C;
			} else if (_size.*C < c->_minSize.*C) {
				c->_size.*C = c->_minSize.*C;
			} else if (_size.*C > c->_maxSize.*C) {
				c->_size.*C = c->_maxSize.*C;

				auto pos = c->position();
				auto dw = (_size.*C - c->_maxSize.*C) / 2;
				pos.*C = dw * (1 + gravityDir) / 2;
				c->position(pos);
			} else {
				LASSERT("We cant reach this")
			}
		}
	};

	if (_layout == UIElement::HORIZONTAL) {
		stacker.template operator()<&Vector2<UIUnit>::x>(gravityToDir(_gravityH));
		placeOrthogonal.template operator()<&Vector2<UIUnit>::y>(gravityToDir(_gravityV));
	} else {
		stacker.template operator()<&Vector2<UIUnit>::y>(gravityToDir(_gravityV));
		placeOrthogonal.template operator()<&Vector2<UIUnit>::x>(gravityToDir(_gravityH));
	}

	for (const auto& c : _childs) {
		c->updateChildsSize();
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

void UIElement::size(Vector2<UIUnit> size) {
	if (size == _size) {
		return;
	}

	auto p = _parent.lock();
	if (!p || p->layout() == UIElement::FREE) {
		_size = size;
	}
}

void UIElement::minSize(Vector2<UIUnit> minSize) {
	if (_minSize == minSize) {
		return;
	}

	_minSize = minSize;
	if (_maxSize.x < minSize.x) {
		_maxSize.x = minSize.x;
	}
	if (_maxSize.y < minSize.y) {
		_maxSize.y = minSize.y;
	}

	auto p = _parent.lock();
	if (!p || p->layout() == UIElement::FREE) {
		if (_size.x < _minSize.x) {
			_size.x = _minSize.x;
		}
		if (_size.y < _minSize.y) {
			_size.y = _minSize.y;
		}
	} else if (p) {
		p->updateChildsSize();
	}
}

void UIElement::maxSize(Vector2<UIUnit> maxSize) {
	if (_maxSize == maxSize) {
		return;
	}

	_maxSize = maxSize;
	if (_minSize.x > maxSize.x) {
		_maxSize.x = maxSize.x;
	}
	if (_minSize.y < maxSize.y) {
		_minSize.y = maxSize.y;
	}

	auto p = _parent.lock();
	if (!p || p->layout() == UIElement::FREE) {
		if (_size.x > _minSize.x) {
			_size.x = _minSize.x;
		}
		if (_size.y > _minSize.y) {
			_size.y = _minSize.y;
		}
	} else if (p) {
		p->updateChildsSize();
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
