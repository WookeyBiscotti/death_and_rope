#pragma once

#include "alch/systems/assets/font.hpp"
#include "alch/systems/broker/sender.hpp"
#include "ui_element.hpp"
//
#include <memory>
#include <string>

class Context;

struct UIButtonOnRelease {};
struct UIButtonOnPress {};

class UIButton: public UIElement, public Sender {
  public:
	UIButton(UIElement* parent, Context& context, std::string content, std::shared_ptr<Font> font);

	void draw(sf::RenderTarget& target) override;

	void onResize() override;
	void onMove() override;

	bool onUnhovered(const UIUnhovered&) override;
	bool onReleased(const UIMouseButtonReleased&) override;
	bool onPressed(const UIMouseButtonPressed&) override;

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