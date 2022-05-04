#include "ui_slider.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

UISlider::UISlider(UIElement* parent, Context& context, float curr, float min, float max, std::shared_ptr<Font> font):
    UIElement(parent, context), Sender(context.systemRef<Broker>()), _font(font), _current(curr),
    _min(min), _max(max) {
	_slider.setFillColor(sf::Color::Green);
}

void UISlider::onResize() {
	onTransform();
}

void UISlider::onMove() {
	onTransform();
}

void UISlider::onTransform() {
	_text.setFont(_font->sf());
	_text.setString(std::to_string(_current));
	_text.setCharacterSize(24);
	_text.setFillColor(sf::Color::Black);

	auto gb = _text.getGlobalBounds();
	auto gp = toWorldCoords(position());

	_text.setOrigin(gb.width / 2, gb.height / 2);
	_text.setPosition(gp + 0.5f * size());

	_bg.setSize(_size);
	_bg.setPosition(gp);

	_slider.setSize(Vector2f(_size.x * 0.05, _size.y));

	_slider.setOrigin(_slider.getSize().x * _current / (_max - _min), 0);
	_slider.setPosition(Vector2f(gp.x + _size.x * _current / (_max - _min), gp.y));
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

	p.x = std::max(std::min(p.x, endPos.x), startPos.x);

	_current = _min + (_max - _min) * (p.x - startPos.x) / (endPos.x - startPos.x);

	onTransform();
}

void UISlider::draw(sf::RenderTarget& target) {
	_bg.setFillColor(sf::Color(100, 150, 200));
	target.draw(_bg);
	target.draw(_slider);
	target.draw(_text);
}
