#include "name.hpp"

#include "name_system.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"

Name::Name(Entity& entity, std::string name): Component(entity), _name(std::move(name)) {
	entity.context().systemRef<NameSystem>().add(_name, &entity);
}

void Name::name(const std::string& name) {
	entity().context().systemRef<NameSystem>().remove(_name);
	entity().context().systemRef<NameSystem>().add(_name, &entity());
}

void Name::serialize(OArchive& ar) const {
	ar(_name);
}

void Name::deserialize(IArchive& ar) {
	if (!_name.empty()) {
		name("");
	}
	ar(_name);
	name(_name);
}
