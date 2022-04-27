#pragma once

#include "alch/systems/assets/font.hpp"
#include "alch/systems/broker/sender.hpp"
#include "ui_element.hpp"
//
#include <memory>
#include <string>

class Context;

class UIText: public UIElement, public Sender {
  public:
	UIText(UIElement* parent, Context& context, std::string content, std::shared_ptr<Font> font);

	void draw(sf::RenderTarget& target) override;

  protected:
	void onResize() override;
	void onMove() override;

	void onTransform();

	bool eventable() const override { return true; }

  protected:
	std::string _content;
	std::shared_ptr<Font> _font;
	sf::RectangleShape _bg;
	sf::Text _text;
};
