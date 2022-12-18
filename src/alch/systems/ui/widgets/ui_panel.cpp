#include "ui_panel.hpp"

#include "alch/systems/logging/logger.hpp"

//
#include <SFML/Graphics/RectangleShape.hpp>

using namespace al;

void UIPanel::draw(sf::RenderTarget& target) {
	using enum StyleName;
	sf::RectangleShape rs;
	rs.setSize(_size);
	rs.setPosition(toWorldCoords(_position));

	rs.setFillColor(style<BACKGROUND_COLOR, Color>());
	rs.setOutlineColor(style<BORDER_COLOR, Color>());
	rs.setOutlineThickness(-style<BORDER_THICKNESS, float>());

	target.draw(rs);

	UIElement::draw(target);
}
