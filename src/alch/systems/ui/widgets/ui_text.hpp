#pragma once

#include "../ui_element.hpp"
#include "alch/systems/assets/font.hpp"
//
#include <string>

namespace al {

class Context;

struct UITextOnDrug {
	Vector2f dr;
};

class UIText: public UIElement {
  public:
	UIText(Context& context, WeakPtr<UIElement> parent, std::string content = "");

	void draw(sf::RenderTarget& target) override;

	void content(const std::string& s);

  protected:
	void onSizeChange() override;
	void onPositionChange() override;

  protected:
	std::string _content;
	SharedPtr<Font> _font;
	sf::Text _text;
};

} // namespace al
