#include "ui_button.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

using namespace al;

UIButtonOld::UIButtonOld(Context& context, WeakPtr<UIElement> parent, std::string content, SharedPtr<Font> font):
    UIElement(context, parent), _content(content), _font(std::move(font)) {
	if (!_font) {
		_font = context.systemRef<AssetCache>().font();
	}

	_bg.setOutlineColor(sf::Color::Black);
	_bg.setOutlineThickness(-3);
}

void UIButtonOld::draw(sf::RenderTarget& target) {
	if (_pressed) {
		drawPressed(target);
	} else {
		drawIdle(target);
	}
}

void UIButtonOld::drawIdle(sf::RenderTarget& target) {
	_bg.setFillColor(sf::Color(200, 200, 200));
	_bg.setOutlineThickness(-3);
	target.draw(_bg);
	target.draw(_text);
}

void UIButtonOld::drawPressed(sf::RenderTarget& target) {
	_bg.setFillColor(sf::Color(100, 100, 100));
	_bg.setOutlineThickness(-6);
	target.draw(_bg);
	target.draw(_text);
}

// void UIButtonOld::onResize() {
// 	onTransform();
// }

// void UIButtonOld::onMove() {
// 	onTransform();
// }

void UIButtonOld::onTransform() {
	if (_font) {
		_text.setFont(_font->sf());
	}
	// sf::String::toUtf8(_content);
	_text.setString(sf::String::fromUtf8(_content.begin(), _content.end()));
	_text.setCharacterSize(14);
	_text.setFillColor(sf::Color::Black);
	// _text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	auto gb = _text.getGlobalBounds();
	auto gp = toWorldCoords(position());

	_text.setOrigin(gb.width / 2, gb.height / 2);
	_text.setPosition(gp + 0.5f * size());

	_bg.setSize(_size);
	_bg.setPosition(gp);
}

void UIButtonOld::onUnhovered(const UIUnhovered&) {
	if (_pressed) {
		_pressed = false;
	}
}

void UIButtonOld::onReleased(const UIMouseButtonReleased&) {
	if (_pressed) {
		_pressed = false;
		send(UIButtonOnRelease{});
	}
}

void UIButtonOld::onPressed(const UIMouseButtonPressed&) {
	_pressed = true;
	send(UIButtonOnPress{});
}
