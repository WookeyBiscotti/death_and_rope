#include "name.hpp"
#include "namer.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>

Name::Name(Entity& entity, std::string name): Component(entity), _name(std::move(name)) {
	entity.context().systemRef<Namer>().add(_name, &entity);
}

void Name::name(const std::string& name) {
	entity().context().systemRef<Namer>().remove(_name);
	entity().context().systemRef<Namer>().add(_name, &entity());
}