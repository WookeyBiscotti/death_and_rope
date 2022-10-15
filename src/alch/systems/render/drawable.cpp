#include "drawable.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/render/render.hpp"

using namespace alch;

Drawable::Drawable(Entity& entity): Component(entity) {
	entity.context().systemRef<Render>().add(this);
}

Drawable::~Drawable() {
	entity().context().systemRef<Render>().remove(this);
}
