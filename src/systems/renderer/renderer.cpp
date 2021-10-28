#include "renderer.hpp"

#include <context.hpp>

#include <SFML/Graphics.hpp>

void Renderer::render()
{
    for (auto& r : _drawables) {
        r->draw(_context.target, RenderStates::Default);
    }
}