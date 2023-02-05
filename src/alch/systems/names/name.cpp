#include "name.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "name_system.hpp"

using namespace al;

Name::Name(Entity& entity, std::string name): Component(entity), _name(std::move(name)) {
	entity.context().systemRef<NameSystem>().add(_name, &entity);
}

void Name::name(const std::string& name) {
	entity().context().systemRef<NameSystem>().remove(_name);
	entity().context().systemRef<NameSystem>().add(_name, &entity());
}

void Name::save(OArchive& archive) const {
	archive(_name);
}
void Name::load(IArchive& archive) {
	if (!_name.empty()) {
		name("");
	}
	archive(_name);
	name(_name);
}
