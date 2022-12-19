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

	UIElement* onUnhovered(const UIUnhovered&) override;
	UIElement* onReleased(const UIMouseButtonReleased&) override;
	UIElement* onPressed(const UIMouseButtonPressed&) override;

  protected:
	virtual void drawIdle(sf::RenderTarget& target);
	virtual void drawPressed(sf::RenderTarget& target);

	void onTransform();

  protected:
	bool _pressed{};
	String _content;
	SharedPtr<Font> _font;
	sf::Text _text;
};

} // namespace al
