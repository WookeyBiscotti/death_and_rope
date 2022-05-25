#pragma once

#include "alch/systems/broker/receiver.hpp"
#include "ui_element.hpp"
#include "ui_field.hpp"
#include "ui_slider.hpp"
//
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

class UIScroll: public UIElement {
  public:
	UIScroll(UIElement* parent, Context& context);

	void layout(UIElement::Layout l) override;

	void add(std::unique_ptr<UIElement> element) override;

  private:
	void onLayoutUpdate();

	void onResize() override;

  private:
	UIElement* _root;
	UIField* _content;
	UISlider* _slider;
};
