#include "ui_checkbox.hpp"

using namespace al;

UICheckBox::UICheckBox(Context& context, WeakPtr<UIElement> parent, bool checked):
    UIElement(context, parent), _checked(checked) {
}

void UICheckBox::draw(sf::RenderTarget& target) {
	using enum StyleName;
	sf::RectangleShape rs;
	rs.setSize(_size);
	rs.setPosition(toWorldCoords(_position));

	rs.setFillColor(style<EDIT_PLACE_COLOR, Color>());
	rs.setOutlineColor(style<BORDER_COLOR, Color>());
	rs.setOutlineThickness(-style<BORDER_THICKNESS, float>());
	target.draw(rs);

	if (_checked) {
		rs.setSize(_size * 0.5f);
		rs.setOutlineThickness(0);
		rs.setFillColor(style<TEXT_COLOR, Color>());
		rs.setPosition(rs.getPosition() + _size * 0.25f);
	}
	target.draw(rs);
}

void UICheckBox::onPressed(const UIMouseButtonPressed&) {
	_checked = !_checked;

	send(UICheckBoxChanged{_checked});
}
