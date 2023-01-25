#pragma once

#include "../ui_element.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/systems/assets/font.hpp"
//
#include <string>

namespace al {

class Context;

class UIChoise: public UIElement {
  public:
	UIChoise(Context& context, WeakPtr<UIElement> parent, const std::vector<String>& choses, std::size_t idx = 0);

	~UIChoise();

	void draw(sf::RenderTarget& target) override;

	void onPressed(const UIMouseButtonPressed&) override;

	bool onMouseWheel(const UIMouseWheel&) override;

  protected:
	void onSizeChange() override;
	void onPositionChange() override;

  protected:
	UIElement* _chosesUI{};
	Vector2f _chosesUIDR{};
	std::size_t _idx{};
	std::vector<String> _choices;

	SharedPtr<Font> _font;
	sf::Text _text;
};

} // namespace al
