#pragma once

#include "drawable.hpp"

#include <memory>
#include <unordered_set>

class Context;

class Render {
  public:
	Render(Context& context);

	void add(std::shared_ptr<Drawable> drawable) { _drawables.insert(std::move(drawable)); }

	void render();

	RenderTarget& target() { return _target; };

  private:
	Context& _context;

	std::unordered_set<std::shared_ptr<Drawable>> _drawables;

	RenderTarget& _target;
};