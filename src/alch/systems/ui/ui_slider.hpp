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
struct UISliderOnValueChange {
	float value;
	float min;
	float max;
};

class UISlider: public UIElement, public Sender {
  public:
	UISlider(UIElement* parent, Context& context, float curr, float min, float max);

	void draw(sf::RenderTarget& target) override;

	void onResize() override;
	void onMove() override;

	bool onDragStart(const UIMouseDragStart&) override;
	bool onDrag(const UIMouseDrag&) override;
	bool onDragStop(const UIMouseDragStop&) override;

	bool eventable() const override { return true; }

	bool vertical() const { return _isVertical; }
	void vertical(bool vertial) { _isVertical = vertial; }

	float value() const { return _current; }
	void value(float value) { _current = std::max(_min, (std::min(value, _max))); }

	float min() const { return _min; }
	float max() const { return _max; }

  protected:
	void onTransform();
	void setValueFromPoint(Vector2f p);

  protected:
	sf::RectangleShape _bg;
	sf::RectangleShape _slider;
	float _current;
	float _min;
	float _max;
	bool _isVertical{};
};
