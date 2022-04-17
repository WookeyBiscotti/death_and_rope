#include "ui_panel.hpp"

#include "alch/systems/logging/logger.hpp"

void UIPanel::draw(sf::RenderTarget& target) {
	auto p = toWorldCoords(_position);
	_shape.setPosition(p);
	_shape.setSize(_size);

	target.draw(_shape);

	for (auto& c : _childs) {
		c->draw(target);
	}
}

bool UIPanel::onHovered(const UIHovered&) {
	return true;
}

bool UIPanel::onUnhovered(const UIUnhovered&) {
	return true;
}

bool UIPanel::onPressed(const UIMouseButtonPressed&) {
	return true;
}

bool UIPanel::onReleased(const UIMouseButtonReleased&) {
	return true;
}
