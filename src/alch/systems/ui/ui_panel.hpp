#pragma once

#include "ui_element.hpp"
//
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

class UIPanel: public UIElement {
  public:
	UIPanel(UIElement* parent, Context& context): UIElement(parent, context) {}

	void draw(sf::RenderTarget& target) override;

	sf::RectangleShape& shape() { return _shape; }

  private:
	sf::RectangleShape _shape;
};
