#pragma once

#include "drawable.hpp"

#include "alch/engine/system.hpp"
//
#include <memory>
#include <unordered_set>

class Context;
class Camera;
class Render: public System {
  public:
	explicit Render(Context& context);

	void add(Drawable* drawable) { _drawables.insert(drawable); }
	void remove(Drawable* drawable) { _drawables.erase(drawable); }

	void camera(Camera* camera) { _camera = camera; }
	Camera* camera() const { return _camera; }

	void render();

	RenderTarget& target() { return _target; };

  private:
	Context& _context;

	Camera* _camera{};

	std::unordered_set<Drawable*> _drawables;

	RenderTarget& _target;
};
