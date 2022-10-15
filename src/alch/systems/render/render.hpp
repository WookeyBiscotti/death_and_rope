#pragma once

#include "alch/engine/system.hpp"
#include "alch/systems/window/window.hpp"
#include "drawable.hpp"
//
#include <memory>
#include <unordered_set>

namespace alch {

class Context;
class Camera;
class Render: public System {
  public:
	explicit Render(Context& context);
	~Render();

	void add(Drawable* drawable) { _drawables.insert(drawable); }
	void remove(Drawable* drawable) { _drawables.erase(drawable); }

	void camera(Camera* camera) { _camera = camera; }
	Camera* camera() const { return _camera; }

	void render();

	RenderWindow& target() { return _target; };

  private:
	Context& _context;

	Camera* _camera{};

	std::unordered_set<Drawable*> _drawables;

	RenderWindow& _target;
};

} // namespace alch
