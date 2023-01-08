#include "ui_window.hpp"

#include "ui_panel.hpp"
#include "ui_text.hpp"

using namespace al;

UIWindow::UIWindow(Context& context, WeakPtr<UIElement> parent, String content): UIProxy(context, parent) {
	_layout = UIElement::VERICAL;
	auto header = SharedPtr<UIText>::make(context, parent, content);
	UIElement::add(header);
	header->maxSize({UIUnitMax, 30});
	auto body = SharedPtr<UIPanel>::make(context, parent);
	UIElement::add(body);

	_proxy = body.get();
	subscribe<UIElementOnDrag>(header.get(), [this](const UIElementOnDrag& e) {
		auto newP = position() + e.dr;
		auto p = _parent.lock();
		LASSERT(p);
		const auto pSize = p->size();
		if (Rect<float>(0, 0, pSize.x - _size.x, pSize.y - _size.y).contains(newP)) {
			position(position() + e.dr);
		}
	});
}
