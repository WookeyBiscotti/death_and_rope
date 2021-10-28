#pragma once

#include <memory>
#include <set>

#include "drawable.hpp"

class Context;

class Renderer
{
public:
    Renderer(Context& context)
        : _context(context)
    {}

    void add(std::shared_ptr<Drawable> drawable) { _drawables.insert(std::move(drawable)); }

    void render();

private:
    std::set<std::shared_ptr<Drawable>> _drawables;

    Context& _context;
};