#pragma once

#include "../ui_element.hpp"

namespace al {

class UIPanel: public UIElement {
  public:
	UIPanel(Context& context, WeakPtr<UIElement> parent): UIElement(context, parent) {}

	void draw(sf::RenderTarget& target) override;
};

} // namespace al
