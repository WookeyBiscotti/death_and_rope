#pragma once

#include "alch/systems/assets/font.hpp"
#include "ui_element.hpp"
//
#include <memory>
#include <string>

namespace al {

class Context;

struct UISliderOnRelease {};
struct UISliderOnPress {};
struct UISliderOnMove {};
struct UISliderOnValueChange {
	float value;
	float min;
	float max;
};

class UISlider: public UIElement {
  public:
	UISlider(Context& context, WeakPtr<UIElement> parent, float curr, float min, float max);

	void draw(sf::RenderTarget& target) override;

	void onResize() override;
	void onMove() override;

	UIElement* onDragStart(const UIMouseDragStart&) override;
	UIElement* onDrag(const UIMouseDrag&) override;

	UIElement* onMouseWheel(const UIMouseWheel& e) override;

	bool vertical() const { return _isVertical; }
	void vertical(bool vertial) { _isVertical = vertial; }

	float value() const { return _current; }
	void value(float value);

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

} // namespace al
