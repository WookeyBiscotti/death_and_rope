#pragma once

#include "ui_element.hpp"
//
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

class UIPanel: public UIElement {
  public:
	UIPanel(UIElement* parent, UISystem& system): UIElement(parent, system) {}

	void draw(sf::RenderTarget& target) override;

	bool onHovered(const UIHovered&) override;
	bool onUnhovered(const UIUnhovered&) override;
	bool onPressed(const UIMouseButtonPressed&) override;
	bool onReleased(const UIMouseButtonReleased&) override;

	bool eventable() const override { return true; }

	sf::RectangleShape& shape() { return _shape; }

  private:
	sf::RectangleShape _shape;
};
