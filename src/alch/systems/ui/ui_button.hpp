#pragma once

#include "alch/systems/assets/font.hpp"
#include "ui_element.hpp"
//
#include <memory>
#include <string>

namespace al {

class Context;

struct UIButtonOnRelease {};
struct UIButtonOnPress {};

class UIButtonOld: public UIElement {
  public:
	UIButtonOld(Context& context, WeakPtr<UIElement> parent, std::string content, SharedPtr<Font> font = nullptr);

	void draw(sf::RenderTarget& target) override;

	// void onResize() override;
	// void onMove() override;

	void onUnhovered(const UIUnhovered&) override;
	void onReleased(const UIMouseButtonReleased&) override;
	void onPressed(const UIMouseButtonPressed&) override;

  protected:
	virtual void drawIdle(sf::RenderTarget& target);
	virtual void drawPressed(sf::RenderTarget& target);

	void onTransform();

  protected:
	bool _pressed{};
	std::string _content;
	SharedPtr<Font> _font;
	sf::RectangleShape _bg;
	sf::Text _text;
};

} // namespace al
