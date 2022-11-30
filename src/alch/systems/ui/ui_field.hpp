#pragma once

#include "ui_element.hpp"
//
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace al {

class UIField: public UIElement {
  public:
	UIField(Context& context, WeakPtr<UIElement> parent);

	void draw(sf::RenderTarget& target) override;

	void internalSize(Vector2f size) { _root->size(size); }

	Vector2f internalSize() const { return _root->size(); }

	const std::vector<SharedPtr<UIElement>>& internalChilds() const;

	void layout(UIElement::Layout l) override;

	void add(SharedPtr<UIElement> element) override;

	void internalPosition(Vector2f r);

  private:
  private:
	UIElement* _root;
};

} // namespace al
