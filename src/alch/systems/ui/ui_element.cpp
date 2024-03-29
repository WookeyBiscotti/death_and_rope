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
	_flags[UIFlags::ENABLE] = true;
	_flags[UIFlags::EVENTABLE] = true;
	_flags[UIFlags::FOCUSED] = false;
}

UIElement::~UIElement() {
	system().remove(this);
}

UISystem& UIElement::system() const {
	return _context.systemRef<UISystem>();
}

void UIElement::onSizeChange() {
	updateChildsPositionSize();
}

void UIElement::onPositionChange() {
	for (const auto& c : _childs) {
		c->onPositionChange();
	}
}

void UIElement::updateChildsPositionSize() {
	constexpr UIUnit EPSILON = 0.01;
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

		auto count = _childs.size();
		const auto freeSpace = _size.*C - ((count > 1 ? count - 1 : 0) * _distanceBetweenChildren + _indentBotRight.*C +
		                                      _indentTopLeft.*C);

		auto w = freeSpace;
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
				w = freeSpace;
				for (const auto& c : _childs) {
					c->_size.*C = c->_maxSize.*C;
					w -= c->_size.*C;
				}

				dw = w / _childs.size();

				w = _indentTopLeft.*C;
				for (const auto& c : _childs) {
					Vector2<UIUnit> pos{};
					pos.*C = w + dw * (1 + gravityDir) / 2;
					c->position(pos);
					w += c->_size.*C + dw;
					if (&c != &_childs.back()) {
						w += _distanceBetweenChildren;
					}
				}

				return;

			} else
				// хватает места и мы можем полностью его заполнить
				while (w < EPSILON) {
					w = freeSpace;

					for (auto& c : _childs) {
						c->_size.*C = c->_minSize.*C;
						w -= c->_minSize.*C;
					}
					if (w <= EPSILON) {
						break;
					}

					UIUnit minSize{UIUnitMax};
					InlinedVector<UIElement*, 16> elms;
					for (auto& c : _childs) {
						elms.push_back(c.get());
						if (c->_size.*C < minSize) {
							minSize = c->_size.*C;
						}
					}

					while (!elms.empty() && w >= EPSILON && minSize != UIUnitMax) {
						dw = w / elms.size();

						UIUnit newMinSize{UIUnitMax};
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
							}
							if (e->_size.*C < newMinSize) {
								newMinSize = e->_size.*C;
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

		w = _indentTopLeft.*C;
		for (const auto& c : _childs) {
			Vector2<UIUnit> pos{};
			pos.*C = w;
			c->position(pos);
			w += c->_size.*C;
			w += _distanceBetweenChildren;
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
		const auto freeSpace = _size.*C - (_indentBotRight.*C + _indentTopLeft.*C);

		for (const auto& c : _childs) {
			if (freeSpace >= c->_minSize.*C && freeSpace <= c->_maxSize.*C) {
				c->_size.*C = freeSpace;
			} else if (freeSpace < c->_minSize.*C) {
				c->_size.*C = c->_minSize.*C;
			} else if (freeSpace > c->_maxSize.*C) {
				c->_size.*C = c->_maxSize.*C;
				auto pos = c->position();
				auto dw = freeSpace - c->_maxSize.*C;
				pos.*C = dw * (1 + gravityDir) / 2;
				c->position(pos);
			} else {
				LASSERT("We cant reach this")
			}
			auto pos = c->position();
			pos.*C += _indentTopLeft.*C;
			c->position(pos);
			c->onPositionChange();
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
		c->onSizeChange();
	}
}

void UIElement::add(SharedPtr<UIElement> element) {
	auto e = element.get();
	element->parent(sharedFromThis());
	_childs.push_back(std::move(element));

	updateChildsPositionSize();
}

void UIElement::parent(WeakPtr<UIElement> parent) {
	_parent = parent;
}

Vector2<UIUnit> UIElement::position() const {
	return _position;
}

void UIElement::position(Vector2<UIUnit> position) {
	if (_position != position) {
		_position = position;
		for (const auto& c : _childs) {
			c->onPositionChange();
		}
		send(UIElementPositionChange{});
		onGlobalPositionChange();
	}
}

void UIElement::updatePositionPart() {
}

const Vector2<Optional<float>>& UIElement::positionPart() const {
	return _positionPart;
}

void UIElement::positionPart(const Vector2<Optional<float>>& position) {
}

void UIElement::size(Vector2<UIUnit> size) {
	if (size == _size) {
		return;
	}

	auto p = _parent.lock();
	if (!p || p->layout() == UIElement::FREE) {
		_size = size;
		updateChildsPositionSize();
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
		updateChildsPositionSize();
	} else if (p) {
		p->updateChildsPositionSize();
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
	if (_minSize.y > maxSize.y) {
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
		p->updateChildsPositionSize();
	}
}

void UIElement::remove(UIElement* element) {
	for (auto it = _childs.begin(); it != _childs.end(); ++it) {
		if (it->get() == element) {
			_childs.erase(it);
			updateChildsPositionSize();
			break;
		}
	}
}

void UIElement::removeAll() {
	_childs.clear();
}

void UIElement::enabled(bool enabled) {
	auto p = _parent.lock();
	if (enabled != _flags[UIFlags::ENABLE] && p) {
		// TODO: use enable falg in layout calculation
		p->updateChildsPositionSize();
	}

	_flags[UIFlags::ENABLE] = enabled;
}

void UIElement::draw(sf::RenderTarget& target) {
	for (auto& c : _childs) {
		if (c->enabled()) {
			c->draw(target);
		}
	}
};

void UIElement::gravityH(GravityH g) {
	if (g == _gravityH) {
		return;
	}

	_gravityH = g;

	auto p = _parent.lock();
	if (p && p->layout() != UIElement::FREE) {
		updateChildsPositionSize();
	}
}

void UIElement::gravityV(GravityV g) {
	if (g == _gravityV) {
		return;
	}

	_gravityV = g;

	auto p = _parent.lock();
	if (p && p->layout() != UIElement::FREE) {
		updateChildsPositionSize();
	}
}

const Styles::Value* UIElement::calculatedStyleValuePtr(StyleName name) const {
	if (auto v = _style.find(name); v != _style.end()) {
		return &v->second.value;
	}
	if (auto v = _cachedStyle.find(name); v != _cachedStyle.end()) {
		return &v->second.value;
	}

	auto p = _parent.lock();
	if (p) {
		auto v = p->calculatedStyleValuePtr(name);
		if (v) {
			_cachedStyle[name].value = *v;
			return v;
		}
	}

	return nullptr;
}

void UIElement::styleClearCache() const {
	_cachedStyle.clear();
	for (const auto& c : _childs) {
		c->styleClearCache();
	}
}

void UIElement::distanceBetweenChildren(UIUnit distanceBetweenChildren) {
	_distanceBetweenChildren = distanceBetweenChildren;
	updateChildsPositionSize();
}

void UIElement::indentLeft(UIUnit indentLeft) {
	_indentTopLeft.x = indentLeft;
	updateChildsPositionSize();
}

void UIElement::indentRight(UIUnit indentRight) {
	_indentBotRight.x = indentRight;
	updateChildsPositionSize();
}

void UIElement::indentTop(UIUnit indentTop) {
	_indentTopLeft.y = indentTop;
	updateChildsPositionSize();
}

void UIElement::indentBot(UIUnit indentBottom) {
	_indentBotRight.y = indentBottom;
	updateChildsPositionSize();
}

bool UIElement::name(String name) {
	auto p = _parent.lock();

	if (p && p->find(name)) {
		return false;
	}
	_name = std::move(name);

	return true;
}

const UIElement* UIElement::find(const String& name) const {
	if (name == _name) {
		return this;
	}
	UIElement* found = nullptr;
	for (const auto& c : _childs) {
		found = c->find(name);
		if (found) {
			break;
		}
	}

	return found;
}

UIElement* UIElement::find(const String& name) {
	return const_cast<UIElement*>(find(name));
}

void UIElement::onGlobalPositionChange() {
	send(UIElementGlobalPositionChange{});
	for (const auto& c : _childs) {
		c->onGlobalPositionChange();
	}
}

void UIElement::save(OArchive& archive) {
}

void UIElement::load(IArchive& archive) {
}
