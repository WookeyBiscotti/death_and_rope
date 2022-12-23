#include "ui_text.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/broker/broker.hpp"
#include "../ui_system.hpp"

using namespace al;

UIText::UIText(Context& context, WeakPtr<UIElement> parent, std::string content, SharedPtr<Font> font):
    UIElement(context, parent), _content(content), _font(std::move(font)) {
	if (!_font) {
		_font = context.systemRef<AssetCache>().font();
	}
}

void UIText::draw(sf::RenderTarget& target) {
	using enum StyleName;

	using enum StyleName;
	sf::RectangleShape rs;
	rs.setSize(_size);
	rs.setPosition(toWorldCoords(_position));

	rs.setFillColor(style<BACKGROUND_COLOR, Color>());
	rs.setOutlineColor(style<BORDER_COLOR, Color>());
	rs.setOutlineThickness(-style<BORDER_THICKNESS, float>());

	target.draw(rs);
	target.draw(_text);
}

void UIText::onSizeChange(const Vector2f& old) {
	if (_font) {
		_text.setFont(_font->sf());
	}
	// sf::String::toUtf8(_content);
	_text.setString(sf::String::fromUtf8(_content.begin(), _content.end()));
	_text.setCharacterSize(style<StyleName::TEXT_SIZE, float>());
	_text.setFillColor(style<StyleName::TEXT_COLOR, Color>());
	// _text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	auto gb = _text.getGlobalBounds();
	auto gp = toWorldCoords(position());

	_text.setOrigin(gb.width / 2, gb.height / 2);
	_text.setPosition(gp + 0.5f * size());
}

void UIText::content(const std::string& s) {
	_content = s;
	onSizeChange({});
}
