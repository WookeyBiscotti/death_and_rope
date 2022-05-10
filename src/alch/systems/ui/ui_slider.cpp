#include "ui_slider.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

UISlider::UISlider(UIElement* parent, Context& context, float curr, float min, float max):
    UIElement(parent, context), Sender(context.systemRef<Broker>()), _current(curr), _min(min), _max(max) {
	_slider.setFillColor(sf::Color::Green);
	_bg.setFillColor(sf::Color(100, 150, 200));
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

	if (_isVertical) {
		_slider.setSize(Vector2f(_size.x, _size.y * 0.05));
		_slider.setOrigin(0, _slider.getSize().y * _current / (_max - _min));
		_slider.setPosition(Vector2f(gp.x, gp.y + _size.y * _current / (_max - _min)));
	} else {
		_slider.setSize(Vector2f(_size.x * 0.05, _size.y));
		_slider.setOrigin(_slider.getSize().x * _current / (_max - _min), 0);
		_slider.setPosition(Vector2f(gp.x + _size.x * _current / (_max - _min), gp.y));
	}
}

bool UISlider::onDragStart(const UIMouseDragStart& e) {
	setValueFromPoint(Vector2f(e.event.x, e.event.y));

	return true;
}

bool UISlider::onDrag(const UIMouseDrag& e) {
	setValueFromPoint(Vector2f(e.event.x, e.event.y));

	return true;
}

bool UISlider::onDragStop(const UIMouseDragStop&) {
	return true;
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

void UISlider::draw(sf::RenderTarget& target) {
	target.draw(_bg);
	target.draw(_slider);
}
