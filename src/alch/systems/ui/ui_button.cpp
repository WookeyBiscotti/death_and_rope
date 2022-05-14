#include "ui_button.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

UIButton::UIButton(UIElement* parent, Context& context, std::string content, std::shared_ptr<Font> font):
    UIElement(parent, context), Sender(context.systemRef<Broker>()), _content(std::move(content)),
    _font(std::move(font)) {
	if (!_font) {
		_font = context.systemRef<AssetCache>().font();
	}

	_bg.setOutlineColor(sf::Color::Black);
	_bg.setOutlineThickness(-3);
}

void UIButton::draw(sf::RenderTarget& target) {
	if (_pressed) {
		drawPressed(target);
	} else {
		drawIdle(target);
	}
}

void UIButton::drawIdle(sf::RenderTarget& target) {
	_bg.setFillColor(sf::Color(200, 200, 200));
	target.draw(_bg);
	target.draw(_text);
}

void UIButton::drawPressed(sf::RenderTarget& target) {
	_bg.setFillColor(sf::Color(100, 100, 100));
	target.draw(_bg);
	target.draw(_text);
}

void UIButton::onResize() {
	onTransform();
}

void UIButton::onMove() {
	onTransform();
}

void UIButton::onTransform() {
	if (_font) {
		_text.setFont(_font->sf());
	}
	_text.setString(_content);
	_text.setCharacterSize(24);
	_text.setFillColor(sf::Color::Black);
	// _text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	auto gb = _text.getGlobalBounds();
	auto gp = toWorldCoords(position());

	_text.setOrigin(gb.width / 2, gb.height / 2);
	_text.setPosition(gp + 0.5f * size());

	_bg.setSize(_size);
	_bg.setPosition(gp);
}

UIElement* UIButton::onUnhovered(const UIUnhovered&) {
	if (_pressed) {
		_pressed = false;
	}

	return this;
}

UIElement* UIButton::onReleased(const UIMouseButtonReleased&) {
	if (_pressed) {
		_pressed = false;
		send(UIButtonOnRelease{});

		return this;
	}

	return nullptr;
}

UIElement* UIButton::onPressed(const UIMouseButtonPressed&) {
	_pressed = true;
	send(UIButtonOnPress{});

	return this;
}
