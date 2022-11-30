#pragma once

#include "ui_element.hpp"
//
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace al {

class UIPanel: public UIElement {
  public:
	UIPanel(Context& context, WeakPtr<UIElement> parent): UIElement(context, parent) {
		_shape.setFillColor(sf::Color(200, 200, 200));
	}

	void draw(sf::RenderTarget& target) override;

	sf::RectangleShape& shape() { return _shape; }

  private:
	sf::RectangleShape _shape;
};

} // namespace al
