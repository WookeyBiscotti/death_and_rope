#include "ui_window.hpp"

#include "ui_panel.hpp"
#include "ui_text.hpp"

using namespace al;

UIWindow::UIWindow(Context& context, WeakPtr<UIElement> parent, String content, bool noHeader):
    UIProxy(context, parent) {
	_layout = UIElement::VERICAL;
	if (!noHeader) {
		auto header = SharedPtr<UIPanel>::make(context, parent);
		header->style<StyleName::FLAT_COLOR>(styleDefault<StyleName::HEADER_COLOR, Color>);
		header->layout(UIElement::HORIZONTAL);
		auto text = header->create<UIText>(content);
		UIElement::add(header);
		header->maxSize({UIUnitMax, style<StyleName::HEADER_HEIGHT, float>()});
		subscribe<UIElementOnDrag>(text, [this](const UIElementOnDrag& e) {
			if (_moveable) {
				auto newP = position() + e.dr;
				auto p = _parent.lock();
				LASSERT(p);
				const auto pSize = p->size();
				if (Rect<float>(0, 0, pSize.x - _size.x, pSize.y - _size.y).contains(newP)) {
					position(position() + e.dr);
				}
			}
		});
	}
	auto body = SharedPtr<UIPanel>::make(context, parent);
	_proxy = body.get();
	UIElement::add(body);
}
