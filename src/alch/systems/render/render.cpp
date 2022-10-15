#include "camera.hpp"
#include "render.hpp"
//
#include <SFML/Graphics.hpp>
#include "alch/engine/context.hpp"
//
#include "alch/systems/window/window.hpp"

using namespace al;

Render::Render(Context& context): _context(context), _target(context.systemRef<Window>().window()) {
}

void Render::render() {
	if (_camera) {
		_camera->update();
		_target.setView(_camera->sf());
	}

	for (auto& r : _drawables) {
		r->draw(_target, RenderStates::Default);
	}
}
