#pragma once

#include "../ui_element.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/systems/assets/font.hpp"
//
#include <string>

namespace al {

class Context;

struct UICheckBoxChanged {
	bool value;
};

class UICheckBox: public UIElement {
  public:
	UICheckBox(Context& context, WeakPtr<UIElement> parent, bool checked = false);

	void draw(sf::RenderTarget& target) override;

	void onPressed(const UIMouseButtonPressed&) override;

  protected:
	bool _checked;
};

} // namespace al
