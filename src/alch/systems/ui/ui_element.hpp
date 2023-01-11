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

namespace UIFlags {
enum {
	ENABLE,
	EVENTABLE,
	FOCUSED,

	COUNT,
};
};

#define AL_UI_DECL_TYPE()                                    \
	type_id_t type() override {                              \
		return TypeId<std::remove_cvref<decltype(*this)>>(); \
	}

#define AL_UI_NAME(NAME)                            \
	std::string_view uName() const override {       \
		static const std::string_view name = #NAME; \
		return name;                                \
	};

#define AL_UI(NAME)   \
	AL_UI_DECL_TYPE() \
	AL_UI_NAME(NAME)

struct UIElementOnHovered {};
struct UIElementOnMouseMove {};
struct UIElementOnUnhovered {};

struct UIElementOnPressed {};
struct UIElementOnReleased {};

struct UIElementOnDragStart {};
struct UIElementOnDrag {
	Vector2f dr;
};
struct UIElementOnDragStop {};

struct UIElementOnMouseWheel {};

struct UIElementOnText {};
struct UIElementOnSpecialText {};

struct UIElementOnFocused {};
struct UIElementOnUnfocused {};

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

	virtual Layout layout() const { return _layout; }
	virtual void layout(Layout layout);

	virtual GravityH gravityH() const { return _gravityH; }
	virtual void gravityH(GravityH g);

	virtual GravityV gravityV() const { return _gravityV; }
	virtual void gravityV(GravityV g);

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

	virtual void onSizeChange();
	virtual void onPositionChange();

	virtual void onHovered(const UIHovered&) { send(UIElementOnHovered{}); }
	virtual void onMouseMove(const UIMouseMove&) { send(UIElementOnMouseMove{}); }
	virtual void onUnhovered(const UIUnhovered&) { send(UIElementOnUnhovered{}); }

	virtual void onPressed(const UIMouseButtonPressed&) { send(UIElementOnPressed{}); }
	virtual void onReleased(const UIMouseButtonReleased&) { send(UIElementOnReleased{}); }

	virtual void onDragStart(const UIMouseDragStart&) { send(UIElementOnDragStart{}); }
	virtual void onDrag(const UIMouseDrag& e) { send(UIElementOnDrag{e.dr}); }
	virtual void onDragStop(const UIMouseDragStop&) { send(UIElementOnDragStop{}); }

	virtual bool onMouseWheel(const UIMouseWheel&) { return false; }

	virtual void onText(const UITextEntered&) { send(UIElementOnText{}); }
	virtual void onSpecialText(const UITextEntered&) { send(UIElementOnSpecialText{}); }

	virtual bool onFocused() { return false; };
	virtual void onUnfocused() { send(UIElementOnUnfocused{}); };

	bool eventable() const { return _flags[UIFlags::EVENTABLE]; }
	void eventable(bool eventable) { _flags[UIFlags::EVENTABLE] = eventable; }

	void enabled(bool enabled);
	bool enabled() const { return _flags[UIFlags::ENABLE]; }

	template<class E>
	bool isEventInside(const E& e) const;
	virtual bool isPointInside(const Vector2f& p) const;
	virtual UIElement* isPointElement(const Vector2f& p);

	UISystem& system() const;

	virtual void updateChildsPositionSize();

	virtual const std::vector<SharedPtr<UIElement>>& childs() const { return _childs; }
	std::vector<SharedPtr<UIElement>>& childs() {
		return const_cast<std::vector<SharedPtr<UIElement>>&>(static_cast<const UIElement*>(this)->childs());
	}

	// style calculated Or default
	template<StyleName name, class T>
	const T& style() const;
	template<StyleName name, class T>
	void style(const T& value);

	void styleClearCache() const;

	virtual void distanceBetweenChildren(UIUnit distanceBetweenChildren);
	virtual UIUnit distanceBetweenChildren() const { return _distanceBetweenChildren; }

	virtual void indentLeft(UIUnit indentLeft);
	virtual UIUnit indentLeft() const { return _indentTopLeft.x; }

	virtual void indentRight(UIUnit indentRight);
	virtual UIUnit indentRight() const { return _indentBotRight.x; }

	virtual void indentTop(UIUnit indentTop);
	virtual UIUnit indentTop() const { return _indentTopLeft.y; }

	virtual void indentBot(UIUnit indentBottom);
	virtual UIUnit indentBot() const { return _indentBotRight.y; }

	virtual type_id_t type() { return TypeId<std::remove_cvref<decltype(*this)>>(); }

	virtual std::string_view uName() const { return "UIElement"; }

	const String& name() const { return _name; }
	// returns true if name assigned, else name already exist
	bool name(String name);

	virtual const UIElement* find(const String& s) const;
	UIElement* find(const String& s);

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

	UIUnit _distanceBetweenChildren{};

	Vector2f _indentTopLeft{};
	Vector2f _indentBotRight{};

	String _name;
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
	auto sPtr = SharedPtr<E>::make(_context, weakFromThis(), std::forward<Args>(args)...);
	auto ptr = sPtr.get();
	add(std::move(sPtr));

	return ptr;
}

inline void UIElement::layout(Layout layout) {
	auto copyOld = _layout;
	if (_layout != layout) {
		_layout = layout;
		updateChildsPositionSize();
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

inline UIElement* UIElement::isPointElement(const Vector2f& p) {
	if (isPointInside(p)) {
		for (const auto& c : _childs) {
			if (auto e = c->isPointElement(p)) {
				return e;
			}
		}
		return this;
	}
	return nullptr;
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
