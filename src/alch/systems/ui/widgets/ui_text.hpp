#pragma once

#include "alch/systems/assets/font.hpp"


#include "../ui_element.hpp"
//
#include <string>

namespace al {

class Context;

class UIText: public UIElement {
  public:
	UIText(Context& context, WeakPtr<UIElement> parent, std::string content = "", SharedPtr<Font> font = nullptr);

	void draw(sf::RenderTarget& target) override;

	void content(const std::string& s);

  protected:
	void onSizeChange(const Vector2f& old) override;
	// void onMove() override;

	// void onTransform();

  protected:
	std::string _content;
	SharedPtr<Font> _font;
	sf::Text _text;
};

}
