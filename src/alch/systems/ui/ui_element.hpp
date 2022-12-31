#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/bitset.hpp"
#include "alch/common/optional.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"
#include "alch/systems/broker/transmitter.hpp"
#include "alch/systems/logging/logger.hpp"
#include "ui_events.hpp"
#include "ui_style.hpp"
//
#include <SFML/Graphics.hpp>

namespace al {

class UISystem;
class Context;

using UIUnit = float;

constexpr UIUnit UIUnitMax = std::numeric_limits<UIUnit>::max();

// using EventsPtrs = Variant<UIHovered*, UIMouseMove*, UIUnhovered*, UIMouseButtonPressed*, UIMouseButtonReleased*,
//     UIMouseDragStart*, UIMouseDrag*, UIMouseDragStop*, UIMouseWheel*>;

namespace UIFlags {
enum {
	ENABLE,
	EVENTABLE,
	FOCUSED,

	COUNT,
};
};

class UIElement: public Transmitter, public EnableSharedFromThis<UIElement> {
	friend class UISystem;

  public:
	enum Layout {
		VERICAL,
		HORIZONTAL,
		FREE,
	};

	enum class GravityV {
		NONE,

		UP,   // ⬆️
		DOWN, // ⬇️

	};

	enum class GravityH {
		NONE,

		LEFT,  // ⬅️
		RIGHT, // ➡️
	};

	UIElement(Context& context, WeakPtr<UIElement> parent);

	virtual ~UIElement();

	WeakPtr<UIElement> parent() const { return _parent; }
	void parent(WeakPtr<UIElement> parent);

	void updatePositionPart();
	const Vector2<Optional<float>>& positionPart() const;
	void positionPart(const Vector2<Optional<float>>& position);

	Vector2<UIUnit> position() const;
	void position(Vector2<UIUnit> position);

	Vector2<UIUnit> globalPosition() { return toWorldCoords(position()); }

	Vector2f size() const { return _size; }
	void size(Vector2<UIUnit> size);

	Vector2f minSize() const { return _minSize; }
	void minSize(Vector2<UIUnit> minSize);

	Vector2f maxSize() const { return _maxSize; }
	void maxSize(Vector2<UIUnit> maxSize);

	Layout layout() const { return _layout; }
	void layout(Layout layout);

	GravityH gravityH() const { return _gravityH; }
	void gravityH(GravityH g);

	GravityV gravityV() const { return _gravityV; }
	void gravityV(GravityV g);

	virtual void add(SharedPtr<UIElement> element);

	template<class E, class... Args>
	E* create(Args&&... args);

	virtual void remove(UIElement* element);

	virtual void removeAll();

	Vector2<UIUnit> toWorldCoords(Vector2<UIUnit> p) const;

	bool isLocalPointIn(Vector2f p) const;

	bool isGlobalPointIn(Vector2f p) const;

	virtual void draw(sf::RenderTarget& target);

	virtual void onLayoutChange(Layout old) {}

	virtual void onSizeChange(const Vector2f& old);

	virtual void onHovered(const UIHovered&) {}
	virtual void onMouseMove(const UIMouseMove&) {}
	virtual void onUnhovered(const UIUnhovered&) {}

	virtual void onPressed(const UIMouseButtonPressed&) {}
	virtual void onReleased(const UIMouseButtonReleased&) {}

	virtual void onDragStart(const UIMouseDragStart&) {}
	virtual void onDrag(const UIMouseDrag&) {}
	virtual void onDragStop(const UIMouseDragStop&) {}

	virtual bool onMouseWheel(const UIMouseWheel&) { return false; }

	virtual void onText(const UITextEntered&) {}
	virtual void onSpecialText(const UITextEntered&) {}

	virtual bool onFocused() { return false; };
	virtual void onUnfocused(){};

	bool eventable() const { return _flags[UIFlags::EVENTABLE]; }
	void eventable(bool eventable) { _flags[UIFlags::EVENTABLE] = eventable; }

	void enabled(bool enabled);
	bool enabled() const { return _flags[UIFlags::ENABLE]; }

	template<class E>
	bool isEventInside(const E& e) const;
	virtual bool isPointInside(const Vector2f& p) const;

	UISystem& system() const;

	virtual void updateChildsPositionSize();

	const auto& childs() const { return _childs; }

	// style calculated Or default
	template<StyleName name, class T>
	const T& style() const;
	template<StyleName name, class T>
	void style(const T& value);

	void styleClearCache() const;

  protected:
	const Styles::Value* calculatedStyleValuePtr(StyleName name) const;

	template<StyleName name, class T>
	void updateCachedStyle(const T& value) const;

  protected:
	Context& _context;

	Layout _layout = FREE;

	WeakPtr<UIElement> _parent;

	Vector2<UIUnit> _position;
	Vector2<Optional<float>> _positionPart;

	Vector2<UIUnit> _size;

	Vector2<UIUnit> _minSize;
	Vector2<Optional<UIUnit>> _minSizePart;

	Vector2<UIUnit> _maxSize = {UIUnitMax, UIUnitMax};
	Vector2<Optional<UIUnit>> _maxSizePart;

	std::vector<SharedPtr<UIElement>> _childs;

	GravityV _gravityV = GravityV::NONE;
	GravityH _gravityH = GravityH::NONE;

	Styles::Map _style;
	mutable Styles::Map _cachedStyle;

	Bitset<UIFlags::COUNT> _flags;
};

template<StyleName name, class T>
void UIElement::style(const T& value) {
	static_assert(std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<decltype(styleDefault<name, T>)>>,
	    "Must be same types");

	_style[name].value = value;
	updateCachedStyle<name>(value);
}

template<StyleName name, class T>
const T& UIElement::style() const {
	auto vo = calculatedStyleValuePtr(name);
	if (vo) {
		auto v = std::get_if<T>(vo);
		if (v) {
			return *v;
		}
	}
	const auto& d = styleDefault<name, T>;
	_cachedStyle[name].value = d;

	return d;
}

inline bool UIElement::isLocalPointIn(Vector2f p) const {
	return p.x >= _position.x && p.x <= _position.x + _size.x && p.y >= _position.y && p.y <= _position.y + _size.y;
}

inline bool UIElement::isGlobalPointIn(Vector2f p) const {
	for (auto par = parent().lock(); par;) {
		p -= par->position();
		par = par->parent().lock();
	}

	return isLocalPointIn(p);
}

inline Vector2<UIUnit> UIElement::toWorldCoords(Vector2<UIUnit> p) const {
	// TODO: hash this
	for (auto par = parent().lock(); par;) {
		p += par->position();
		par = par->parent().lock();
	}

	return p;
}

template<class E, class... Args>
E* UIElement::create(Args&&... args) {
	auto sPtr = SharedPtr<E>::make(_context, sharedFromThis(), std::forward<Args>(args)...);
	auto ptr = sPtr.get();
	add(std::move(sPtr));

	return ptr;
}

inline void UIElement::layout(Layout layout) {
	auto copyOld = _layout;
	if (_layout != layout) {
		_layout = layout;
		onLayoutChange(copyOld);
	}
}

template<class E>
bool UIElement::isEventInside(const E& e) const {

	return isPointInside(Vector2f(e.event.x, e.event.y));
}

inline bool UIElement::isPointInside(const Vector2f& p) const {
	const auto globalPos = toWorldCoords(_position);

	return globalPos.x <= p.x && p.x <= globalPos.x + _size.x && globalPos.y <= p.y && p.y <= globalPos.y + _size.y;
}

template<StyleName name, class T>
void UIElement::updateCachedStyle(const T& v) const {
	if (auto f = _cachedStyle.find(name); f != _cachedStyle.end()) {
		f->second.value = v;
	} else {
		for (const auto& c : _childs) {
			c->updateCachedStyle<name>(v);
		}
	}
}

} // namespace al
