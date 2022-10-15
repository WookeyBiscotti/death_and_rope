#include "ui_panel.hpp"

#include "alch/systems/logging/logger.hpp"

using namespace al;

void UIPanel::draw(sf::RenderTarget& target) {
	auto p = toWorldCoords(_position);
	_shape.setPosition(p);
	_shape.setSize(_size);

	target.draw(_shape);

	for (auto& c : _childs) {
		c->draw(target);
	}
}
