#pragma once

#include "alch/systems/assets/font.hpp"
#include "alch/systems/broker/sender.hpp"
#include "ui_element.hpp"
//
#include <memory>
#include <string>

namespace al {

class Context;

struct UIButtonOnRelease {};
struct UIButtonOnPress {};

class UIButton: public UIElement{
  public:
	UIButton(UIElement* parent, Context& context, std::string content, std::shared_ptr<Font> font = nullptr);

	void draw(sf::RenderTarget& target) override;

	void onResize() override;
	void onMove() override;

	UIElement* onUnhovered(const UIUnhovered&) override;
	UIElement* onReleased(const UIMouseButtonReleased&) override;
	UIElement* onPressed(const UIMouseButtonPressed&) override;

  protected:
	virtual void drawIdle(sf::RenderTarget& target);
	virtual void drawPressed(sf::RenderTarget& target);

	void onTransform();

  protected:
	bool _pressed{};
	std::string _content;
	std::shared_ptr<Font> _font;
	sf::RectangleShape _bg;
	sf::Text _text;
};

}
