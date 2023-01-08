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
	UIWindow(Context& context, WeakPtr<UIElement> parent, String content);

	// void draw(sf::RenderTarget& target) override;

  protected:
	// void onSizeChange(const Vector2f& old) override;

	enum class Flags {
		IDLE,
		FOCUSED,
		SELECTING,
	} _state = Flags::IDLE;

  protected:
	String _content;
	SharedPtr<Font> _font;
	sf::Text _text;
};

} // namespace al
