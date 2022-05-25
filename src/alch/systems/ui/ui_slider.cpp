#include "ui_slider.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

UISlider::UISlider(UIElement* parent, Context& context, float curr, float min, float max):
    UIElement(parent, context), _current(curr), _min(min), _max(max) {
	_slider.setFillColor(sf::Color(200, 200, 200));
	_slider.setOutlineColor(sf::Color::Black);
	_slider.setOutlineThickness(-3);
	_bg.setFillColor(sf::Color(100, 100, 100));
	_bg.setOutlineColor(sf::Color::Black);
	_bg.setOutlineThickness(-1);
}

void UISlider::onResize() {
	onTransform();
}

void UISlider::onMove() {
	onTransform();
}

void UISlider::onTransform() {
	auto gp = toWorldCoords(position());

	_bg.setSize(_size);
	_bg.setPosition(gp);

	const auto n = (_current - _min) / (_max - _min);
	if (_isVertical) {
		_slider.setSize(Vector2f(_size.x, _size.y * 0.05));
		_slider.setOrigin(0, _slider.getSize().y * n);
		_slider.setPosition(Vector2f(gp.x, gp.y + _size.y * n));
	} else {
		_slider.setSize(Vector2f(_size.x * 0.05, _size.y));
		_slider.setOrigin(_slider.getSize().x * n, 0);
		_slider.setPosition(Vector2f(gp.x + _size.x * n, gp.y));
	}
}

UIElement* UISlider::onDragStart(const UIMouseDragStart& e) {
	setValueFromPoint(Vector2f(e.event.x, e.event.y));

	return this;
}

UIElement* UISlider::onDrag(const UIMouseDrag& e) {
	setValueFromPoint(Vector2f(e.event.x, e.event.y));

	return this;
}

void UISlider::setValueFromPoint(Vector2f p) {
	auto startPos = toWorldCoords(_position);
	auto endPos = toWorldCoords(_position + _size);

	if (_isVertical) {
		p.y = std::max(std::min(p.y, endPos.y), startPos.y);
		_current = _min + (_max - _min) * (p.y - startPos.y) / (endPos.y - startPos.y);
	} else {
		p.x = std::max(std::min(p.x, endPos.x), startPos.x);
		_current = _min + (_max - _min) * (p.x - startPos.x) / (endPos.x - startPos.x);
	}

	onTransform();

	send<UISliderOnValueChange>({_current, _min, _max});
}

void UISlider::value(float value) {
	_current = std::max(_min, (std::min(value, _max)));
	onTransform();
	send<UISliderOnValueChange>({_current, _min, _max});
}

void UISlider::draw(sf::RenderTarget& target) {
	target.draw(_bg);
	target.draw(_slider);
}

UIElement* UISlider::onMouseWheel(const UIMouseWheel& e) {
	value(value() - (e.event.delta > 0 ? 1 : -1));

	return this;
}
