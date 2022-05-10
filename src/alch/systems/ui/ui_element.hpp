#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"
#include "alch/systems/broker/sender.hpp"
#include "ui_events.hpp"
//
#include <SFML/Graphics.hpp>
#include <string>

class UISystem;
class Context;

class UIElement {
	friend class UISystem;

  public:
	enum Layout {
		VERICAL,
		HORIZONTAL,
		FREE,
	};
	std::string _debug;

	UIElement(UIElement* parent, Context& context);

	virtual ~UIElement();

	UIElement* parent() const { return _parent; }
	void parent(UIElement* parent) {
		_parent = parent;
		onResize();
		onMove();
	}

	Vector2f position() const { return _position; }
	void position(Vector2f position) {
		_position = position;
		onMove();
	}

	virtual Vector2f size() const { return _size; }
	virtual void size(Vector2f size, bool noParentCallback = false) {
		if (size == _size) {
			return;
		}
		_size = size;
		onResize();
		if (!noParentCallback && _parent) {
			_parent->onResize();
		}
	}

	void enabled(bool enabled) { _enabled = enabled; }
	bool enabled() const { return _enabled; }

	void resizeable(bool resizeable) {
		_resizeable = resizeable;
		if (_parent) {
			_parent->onResize();
		}
	}
	bool resizeable() const { return _resizeable; }

	virtual Layout layout() const { return _layout; }
	virtual void layout(Layout layout) { _layout = layout; }

	bool focused() const { return _focused; }

	virtual void add(std::unique_ptr<UIElement> element);
	void add(UIElement* element);

	template<class E, class... Args>
	E* create(Args&&... args) {
		auto sPtr = std::make_unique<E>(this, _context, std::forward<Args>(args)...);
		auto ptr = sPtr.get();
		add(std::move(sPtr));

		return ptr;
	}

	virtual void remove(UIElement* element) {
		for (auto it = _childs.begin(); it != _childs.end(); ++it) {
			if (it->get() == element) {
				_childs.erase(it);
				break;
			}
		}
	}

	Vector2f toWorldCoords(Vector2f p) {
		for (auto par = parent(); par;) {
			p += par->position();
			par = par->parent();
		}

		return p;
	}

	bool isLocalPointIn(Vector2f p) const {
		return p.x >= _position.x && p.x <= _position.x + _size.x && p.y >= _position.y && p.y <= _position.y + _size.y;
	}

	bool isGlobalPointIn(Vector2f p) const {
		for (auto par = parent(); par;) {
			p -= par->position();
			par = par->parent();
		}

		return isLocalPointIn(p);
	}

	virtual void draw(sf::RenderTarget& target) {
		for (auto& c : _childs) {
			c->draw(target);
		}
	};

	virtual void onResize();
	virtual void onMove();

	virtual bool onHovered(const UIHovered&) { return false; }
	virtual bool onUnhovered(const UIUnhovered&) { return false; }

	virtual bool onPressed(const UIMouseButtonPressed&) { return false; }
	virtual bool onReleased(const UIMouseButtonReleased&) { return false; }

	virtual bool onDragStart(const UIMouseDragStart&) { return false; }
	virtual bool onDrag(const UIMouseDrag&) { return false; }
	virtual bool onDragStop(const UIMouseDragStop&) { return false; }

	virtual bool eventable() const { return false; }

	virtual Vector2f wantsSize() const { return {}; }

	UISystem& system() const;

  private:
	void focused(bool focused) { _focused = focused; }

  protected:
	Context& _context;

	UIElement* _parent;

	Vector2f _position;
	Vector2f _size;
	Layout _layout = FREE;

	std::vector<std::unique_ptr<UIElement>> _childs;

  private:
	bool _resizeable = true;
	bool _focused = false;
	bool _enabled = true;
};
