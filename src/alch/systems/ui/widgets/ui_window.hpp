#pragma once

#include "../ui_element.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/systems/assets/font.hpp"
#include "ui_proxy.hpp"
//
namespace al {

class Context;

class UIWindow: public UIProxy {
  public:
	UIWindow(Context& context, WeakPtr<UIElement> parent, String content, bool noHeader = false /*refactor*/);

	void moveable(bool moveable) { _moveable = moveable; }

  protected:
	bool _moveable{true};
	String _content;
	SharedPtr<Font> _font;
	sf::Text _text;
};

} // namespace al
