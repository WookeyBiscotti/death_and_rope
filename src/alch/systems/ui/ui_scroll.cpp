#include "ui_scroll.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"

using namespace al;

UIScroll::UIScroll(Context& context, WeakPtr<UIElement> parent): UIElement(context, parent) {
	_layout = UIElement::HORIZONTAL;

	auto content = SharedPtr<UIField>::make(context, sharedFromThis());
	auto slider = SharedPtr<UISlider>::make(context, sharedFromThis(), 0, 0, 10);

	_content = content.get();
	_slider = slider.get();

	_childs.push_back(std::move(content));
	_childs.push_back(std::move(slider));

	// _slider->resizeable(false);
	// _content->resizeable(true);

	onLayoutUpdate();

	subscribe<UISliderOnValueChange>(_slider, [this](const UISliderOnValueChange& e) {
		if (e.min >= e.max) {
			return;
		}

		auto n = (e.value - e.min) / (e.max - e.min);

		const auto internalSize = _content->internalSize();
		const auto externalSize = _content->size();

		if (_layout == UIElement::VERICAL) {
			const auto dx = internalSize.x - externalSize.x;
			if (dx > 0) {
				_content->internalPosition({-n * dx, 0});
			}
		} else {
			const auto dy = internalSize.y - externalSize.y;
			if (dy > 0) {
				_content->internalPosition({0, -n * dy});
			}
		}
	});
}

void UIScroll::onLayoutUpdate() {
	if (_layout == UIElement::HORIZONTAL) {
		_slider->vertical(true);
	} else {
		_slider->vertical(false);
	}
	_slider->size({10, 10});

	onResize();
}

void UIScroll::onResize() {
	UIElement::onResize();
	// calculate content size
	float w{}, h{};
	if (_layout == UIElement::VERICAL) {
		h = _content->size().y;
		for (auto& c : _content->internalChilds()) {
			w += c->size().x;
		}
		w = std::max(w, _size.x);
	} else {
		w = _content->size().x;
		for (auto& c : _content->internalChilds()) {
			h += c->size().y;
		}
		h = std::max(h, _size.y);
	}

	_content->internalSize({w, h});
}

void UIScroll::add(SharedPtr<UIElement> element) {
	_content->add(std::move(element));
}

void UIScroll::layout(UIElement::Layout l) {
	if (l == UIElement::HORIZONTAL) {
		_layout = UIElement::VERICAL;
	} else if (l == UIElement::VERICAL) {
		_layout = UIElement::HORIZONTAL;
	} else {
		return;
	}

	_content->layout(l);
	onLayoutUpdate();
}
