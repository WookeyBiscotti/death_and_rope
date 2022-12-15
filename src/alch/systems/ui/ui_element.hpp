#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/optional.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"
#include "alch/systems/broker/transmitter.hpp"
#include "ui_events.hpp"
//
#include <SFML/Graphics.hpp>
#include <bitset>
#include <string>

namespace al {

class UISystem;
class Context;

using UIUnit = float;

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
	const Vector2<Opt<float>>& positionPart() const;
	void positionPart(const Vector2<Opt<float>>& position);

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

	virtual void add(SharedPtr<UIElement> element);

	template<class E, class... Args>
	E* create(Args&&... args);

	virtual void remove(UIElement* element);

	virtual void removeAll();

	Vector2<UIUnit> toWorldCoords(Vector2<UIUnit> p);

	bool isLocalPointIn(Vector2f p) const;

	bool isGlobalPointIn(Vector2f p) const;

	virtual void draw(sf::RenderTarget& target);

	virtual void onResize() {}
	virtual void onMove();
	virtual void onLayoutChange(Layout old) {}

	virtual UIElement* onHovered(const UIHovered&);
	virtual UIElement* onMouseMove(const UIMouseMove&);
	virtual UIElement* onUnhovered(const UIUnhovered&);

	virtual UIElement* onPressed(const UIMouseButtonPressed&);
	virtual UIElement* onReleased(const UIMouseButtonReleased&);

	virtual UIElement* onDragStart(const UIMouseDragStart&);
	virtual UIElement* onDrag(const UIMouseDrag&);
	virtual UIElement* onDragStop(const UIMouseDragStop&);

	virtual UIElement* onMouseWheel(const UIMouseWheel&);

	bool eventable() const { return _eventable && _enabled; }
	void eventable(bool eventable) { _eventable = eventable; }

	void enabled(bool enabled);
	bool enabled() const { return _enabled; }

	template<class E>
	bool eventInside(const E& e);

	UISystem& system() const;

	virtual void updateChildsSize();

	const auto& childs() const { return _childs; }

  protected:
	Context& _context;

	Layout _layout = FREE;

	WeakPtr<UIElement> _parent;

	Vector2<UIUnit> _position;
	Vector2<Opt<float>> _positionPart;

	Vector2<UIUnit> _size;

	Vector2<UIUnit> _minSize;
	Vector2<Opt<UIUnit>> _minSizePart;

	Vector2<UIUnit> _maxSize = {std::numeric_limits<UIUnit>::max(), std::numeric_limits<UIUnit>::max()};
	Vector2<Opt<UIUnit>> _maxSizePart;

	std::vector<SharedPtr<UIElement>> _childs;

	GravityV _gravityV = GravityV::NONE;
	GravityH _gravityH = GravityH::NONE;

  private:
	bool _enabled = true;
	bool _eventable = true;
};

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

inline Vector2<UIUnit> UIElement::toWorldCoords(Vector2<UIUnit> p) {
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
bool UIElement::eventInside(const E& e) {
	const auto globalPos = toWorldCoords(_position);

	return globalPos.x <= e.event.x && e.event.x <= globalPos.x + _size.x && globalPos.y <= e.event.y &&
	       e.event.y <= globalPos.y + _size.y;
}

} // namespace al
