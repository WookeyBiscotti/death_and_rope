#pragma once

#include "alch/systems/assets/font.hpp"
#include "alch/systems/broker/sender.hpp"
#include "ui_element.hpp"
//
#include <memory>
#include <string>

class Context;

struct UISliderOnRelease {};
struct UISliderOnPress {};
struct UISliderOnMove {};

class UISlider: public UIElement, public Sender {
  public:
	UISlider(UIElement* parent, Context& context, float curr, float min, float max, std::shared_ptr<Font> font);

	void draw(sf::RenderTarget& target) override;

	void onResize() override;
	void onMove() override;

	bool onDragStart(const UIMouseDragStart&) override;
	bool onDrag(const UIMouseDrag&) override;
	bool onDragStop(const UIMouseDragStop&) override;

	bool eventable() const override { return true; }

  protected:
	void onTransform();
	void setValueFromPoint(Vector2f p);

  protected:
	sf::RectangleShape _bg;
	sf::RectangleShape _slider;
	sf::Text _text;
	std::shared_ptr<Font> _font;
	float _current;
	float _min;
	float _max;
};
