#include "drawable.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <systems/render/render.hpp>

Drawable::Drawable(Entity& entity): Component(entity) {
	entity.context().systemRef<Render>().add(this);
}

Drawable::~Drawable() {
	entity().context().systemRef<Render>().remove(this);
}