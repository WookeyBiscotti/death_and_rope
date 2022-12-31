#pragma once

#include "../ui_element.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/systems/assets/font.hpp"
//
#include <string>

namespace al {

class Context;

class UIInput: public UIElement {
  public:
	UIInput(Context& context, WeakPtr<UIElement> parent, String content, SharedPtr<Font> font = nullptr);

	void draw(sf::RenderTarget& target) override;

	// void onReleased(const UIMouseButtonReleased&) override;
	// void onPressed(const UIMouseButtonPressed&) override;

	// void onSizeChange(const Vector2f&) override;

	bool onFocused() override { return true; }
	void onText(const UITextEntered& e) override;
	void onSpecialText(const UITextEntered& e) override;

  protected:
	void onSizeChange(const Vector2f& old) override;

  protected:
	enum class State {
		IDLE,
		FOCUSED,
		SELECTING,
	} _state = State::IDLE;

	String _content;
	SharedPtr<Font> _font;
	sf::Text _text;
};

} // namespace al
