#pragma once

#include "../ui_element.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/systems/assets/font.hpp"
//
#include <string>

namespace al {

class Context;

struct UIButtonOnRelease {};
struct UIButtonOnPress {};

class UIButton: public UIElement {
  public:
	UIButton(Context& context, WeakPtr<UIElement> parent, std::string content, SharedPtr<Font> font = nullptr);

	void draw(sf::RenderTarget& target) override;

	void onResize() override;
	void onMove() override;

	void onUnhovered(const UIUnhovered&) override;
	void onHovered(const UIHovered&) override;
	void onReleased(const UIMouseButtonReleased&) override;
	void onPressed(const UIMouseButtonPressed&) override;

  protected:
	virtual void drawIdle(sf::RenderTarget& target);
	virtual void drawHovered(sf::RenderTarget& target);
	virtual void drawPressed(sf::RenderTarget& target);

	void onTransform();

  protected:
	enum class State {
		IDLE,
		HOVERED,
		PRESSED,
	} _state = State::IDLE;

	String _content;
	SharedPtr<Font> _font;
	sf::Text _text;
};

} // namespace al
