#include "ui_field.hpp"

using namespace al;

class FieldRoot: public UIElement {
  public:
	FieldRoot(Context& context, WeakPtr<UIElement> parent): UIElement(context, parent) {}

	void onResize() override {
		if (_layout == UIElement::VERICAL) {
			float h{};
			for (auto& c : _childs) {
				h += c->size().y;
			}
			size({_size.x, h});
		} else if (_layout == UIElement::HORIZONTAL) {
			float w{};
			for (auto& c : _childs) {
				w += c->size().x;
			}
			size({w, _size.y});
		}
		UIElement::onResize();
	}
};

UIField::UIField(Context& context, WeakPtr<UIElement> parent): UIElement(context, parent) {
	auto root = SharedPtr<FieldRoot>::make(context, sharedFromThis());
	_root = root.get();
	_root->layout(UIElement::FREE);
	_layout = UIElement::FREE;
	UIElement::add(std::move(root));
}

void UIField::layout(UIElement::Layout l) {
	_root->layout(l);
}

void UIField::add(SharedPtr<UIElement> element) {
	if (_root->layout() == UIElement::VERICAL) {
		size({_size.x, _size.y + element->size().y});
	} else if (_root->layout() == UIElement::HORIZONTAL) {
		size({_size.x + element->size().x, _size.y});
	}

	_root->add(std::move(element));
}

void UIField::draw(sf::RenderTarget& target) {
	const auto saveView = target.getView();
	const auto s = target.getSize();
	const auto curPos = toWorldCoords(_position);

	sf::View local;

	auto rs = sf::FloatRect(curPos.x / s.x, curPos.y / s.y, _size.x / s.x, _size.y / s.y);
	local.setViewport(rs);
	local.setCenter(curPos + 0.5f * _size);
	local.setSize(saveView.getSize().x * rs.width, saveView.getSize().y * rs.height);
	target.setView(local);

	_root->draw(target);

	target.setView(saveView);
}

void UIField::internalPosition(Vector2f r) {
	_root->position(r);
}

const std::vector<SharedPtr<UIElement>>& UIField::internalChilds() const {
	return _childs;
}
