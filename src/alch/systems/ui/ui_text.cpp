#include "ui_text.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/broker/broker.hpp"
#include "ui_system.hpp"

UIText::UIText(UIElement* parent, Context& context, std::string content, std::shared_ptr<Font> font):
    UIElement(parent, context), Sender(context.systemRef<Broker>()), _content(std::move(content)),
    _font(std::move(font)) {
	if (!_font) {
		_font = context.systemRef<AssetCache>().font();
	}
	_bg.setOutlineColor(sf::Color::Black);
	_bg.setOutlineThickness(-3);
	_bg.setFillColor(sf::Color(250, 250, 250));
}

void UIText::draw(sf::RenderTarget& target) {
	target.draw(_bg);
	target.draw(_text);
}

void UIText::onResize() {
	onTransform();
}

void UIText::onMove() {
	onTransform();
}

void UIText::onTransform() {
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
