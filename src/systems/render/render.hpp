#pragma once

#include "drawable.hpp"

#include <memory>
#include <unordered_set>

class Context;

class Render {
  public:
	explicit Render(Context& context);

	void add(Drawable* drawable) { _drawables.insert(drawable); }
	void remove(Drawable* drawable) { _drawables.erase(drawable); }

	void render();

	RenderTarget& target() { return _target; };

  private:
	Context& _context;

	std::unordered_set<Drawable*> _drawables;

	RenderTarget& _target;
};