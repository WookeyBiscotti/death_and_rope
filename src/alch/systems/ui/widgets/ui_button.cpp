#include "ui_button.hpp"

#include "../ui_system.hpp"
#include "alch/engine/context.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/broker/broker.hpp"

using namespace al;

UIButton::UIButton(Context& context, WeakPtr<UIElement> parent, std::string content):
    UIElement(context, parent), _content(std::move(content)) {
	if (!_font) {
		_font = context.systemRef<AssetCache>().font(style<StyleName::FONT, String>());
	}
}

void UIButton::draw(sf::RenderTarget& target) {
	if (_state == State::PRESSED) {
		drawPressed(target);
	} else if (_state == State::IDLE) {
		drawIdle(target);
	} else {
		drawHovered(target);
	}
}

void UIButton::drawHovered(sf::RenderTarget& target) {
	using enum StyleName;
	sf::RectangleShape rs;
	rs.setSize(_size);
	rs.setPosition(toWorldCoords(_position));

	rs.setFillColor(style<FOREGROUND_COLOR2, Color>());
	rs.setOutlineColor(style<BORDER_COLOR, Color>());
	rs.setOutlineThickness(-style<BORDER_THICKNESS, float>());

	target.draw(rs);
	target.draw(_text);
}

void UIButton::drawIdle(sf::RenderTarget& target) {
	using enum StyleName;
	sf::RectangleShape rs;
	rs.setSize(_size);
	rs.setPosition(toWorldCoords(_position));

	rs.setFillColor(style<FOREGROUND_COLOR, Color>());
	rs.setOutlineColor(style<BORDER_COLOR, Color>());
	rs.setOutlineThickness(-style<BORDER_THICKNESS, float>());

	target.draw(rs);
	target.draw(_text);
}

void UIButton::drawPressed(sf::RenderTarget& target) {
	using enum StyleName;
	sf::RectangleShape rs;
	rs.setSize(_size);
	rs.setPosition(toWorldCoords(_position));

	rs.setFillColor(style<BACKGROUND_COLOR2, Color>());
	rs.setOutlineColor(style<BORDER_COLOR, Color>());
	rs.setOutlineThickness(-style<BORDER_THICKNESS, float>());

	target.draw(rs);
	target.draw(_text);
}

void UIButton::onUnhovered(const UIUnhovered&) {
	_state = State::IDLE;
}

void UIButton::onHovered(const UIHovered&) {
	_state = State::HOVERED;
}

void UIButton::onReleased(const UIMouseButtonReleased& e) {
	if (_state == State::PRESSED) {
		_state = State::IDLE;
		if (isEventInside(e)) {
			send(UIElementOnReleased{});
		}
	}
}

void UIButton::onPressed(const UIMouseButtonPressed&) {
	_state = State::PRESSED;
	send(UIElementOnPressed{});
}


void UIButton::onSizeChange() {
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

void UIButton::onPositionChange() {
	auto gp = toWorldCoords(position());
	_text.setPosition(gp + 0.5f * size());
}
