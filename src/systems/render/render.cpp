#include "render.hpp"

#include <SFML/Graphics.hpp>
#include <engine/context.hpp>
//
#include <systems/window/window.hpp>

Render::Render(Context& context): _context(context), _target(context.systemRef<Window>().window()) {
}

void Render::render() {
	for (auto& r : _drawables) {
		r->draw(_target, RenderStates::Default);
	}
}