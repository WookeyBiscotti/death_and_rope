#include "ui_proxy.hpp"

using namespace al;

UIProxy::UIProxy(Context& context, WeakPtr<UIElement> parent): UIElement(context, parent) {
}

UIElement::UIElement::Layout UIProxy::layout() const {
	if (_proxy) {
		return _proxy->layout();
	}
	return UIElement::UIElement::Layout::FREE;
}

void UIProxy::layout(UIElement::Layout layout) {
	if (_proxy) {
		_proxy->layout(layout);
	}
}

UIElement::GravityH UIProxy::gravityH() const {
	if (_proxy) {
		return _proxy->gravityH();
	}
	return UIElement::GravityH::NONE;
}

void UIProxy::gravityH(UIElement::GravityH g) {
	if (_proxy) {
		_proxy->gravityH(g);
	}
}

UIElement::GravityV UIProxy::gravityV() const {
	if (_proxy) {
		return _proxy->gravityV();
	}
	return UIElement::GravityV::NONE;
}

void UIProxy::gravityV(UIElement::GravityV g) {
	if (_proxy) {
		_proxy->gravityV(g);
	}
}

void UIProxy::remove(UIElement* element) {
	if (_proxy) {
		_proxy->remove(element);
	}
}

void UIProxy::removeAll() {
	if (_proxy) {
		_proxy->removeAll();
	}
}

void UIProxy::draw(sf::RenderTarget& target) {
	UIElement::draw(target);
}

const std::vector<SharedPtr<UIElement>>& UIProxy::childs() const {
	if (_proxy) {
		return _proxy->childs();
	}
	static std::vector<SharedPtr<UIElement>> empty;
	return empty;
}

void UIProxy::distanceBetweenChildren(UIUnit distanceBetweenChildren) {
	if (_proxy) {
		_proxy->distanceBetweenChildren(distanceBetweenChildren);
	}
}

UIUnit UIProxy::distanceBetweenChildren() const {
	if (_proxy) {
		return _proxy->distanceBetweenChildren();
	}
	return {};
}

void UIProxy::indentLeft(UIUnit indentLeft) {
	if (_proxy) {
		_proxy->indentLeft(indentLeft);
	}
}
UIUnit UIProxy::indentLeft() const {
	if (_proxy) {
		return _proxy->indentLeft();
	}
	return {};
}

void UIProxy::indentRight(UIUnit indentRight) {
	if (_proxy) {
		_proxy->indentRight(indentRight);
	}
}
UIUnit UIProxy::indentRight() const {
	if (_proxy) {
		return _proxy->indentRight();
	}
	return {};
}

void UIProxy::indentTop(UIUnit indentTop) {
	if (_proxy) {
		_proxy->indentTop(indentTop);
	}
}
UIUnit UIProxy::indentTop() const {
	if (_proxy) {
		return _proxy->indentTop();
	}
	return {};
}

void UIProxy::indentBot(UIUnit indentBottom) {
	if (_proxy) {
		_proxy->indentBot(indentBottom);
	}
}
UIUnit UIProxy::indentBot() const {
	if (_proxy) {
		return _proxy->indentBot();
	}
	return {};
}

void UIProxy::add(SharedPtr<UIElement> element) {
	if (_proxy) {
		_proxy->add(element);
	}
}
