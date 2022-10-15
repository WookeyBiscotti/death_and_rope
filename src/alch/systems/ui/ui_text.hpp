#pragma once

#include "alch/systems/assets/font.hpp"
#include "alch/systems/broker/sender.hpp"
#include "alch/systems/broker/receiver.hpp"
#include "ui_element.hpp"
//
#include <memory>
#include <string>

namespace al {

class Context;

class UIText: public UIElement {
  public:
	UIText(UIElement* parent, Context& context, std::string content, std::shared_ptr<Font> font = nullptr);

	void draw(sf::RenderTarget& target) override;

	void value(const std::string& s);

  protected:
	void onResize() override;
	void onMove() override;

	void onTransform();

  protected:
	std::string _content;
	std::shared_ptr<Font> _font;
	sf::RectangleShape _bg;
	sf::Text _text;
};

}
