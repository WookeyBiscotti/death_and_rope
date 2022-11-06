#include "group.hpp"

#include "alch/common/archive.hpp"
#include "alch/engine/entity.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/transform/events.hpp"
#include "alch/systems/transform/transform.hpp"

using namespace al;

Group::Group(Entity& entity): Component(entity), Transmitter(entity.context().systemRef<Broker>()) {
}

Group::Group(Entity& entity, SyncMove_t):
    Component(entity), _moveChilds(true), Transmitter(entity.context().systemRef<Broker>()) {
	subscribe<PositionUpdate>(&entity, [this](const PositionUpdate& p) {
		// TODO: use subscription
		if (!_moveChilds) {
			return;
		}
		for (auto& it : _childs) {
			it.first->ref<Transform>().p(it.first->ref<Transform>().p() + (p.neW - p.old));
		}
	});
}

SharedPtr<Entity> Group::create() {
	auto e = SharedPtr<Entity>::make(entity().context());
	add(e);

	return e;
}

void Group::add(SharedPtr<Entity> entity) {
	auto ptr = entity.get();
	_childs.emplace(ptr, std::move(entity));
}

void Group::remove(Entity* entity) {
	_childs.erase(entity);
}

void Group::deserialize(IArchive& ar) {
	ar(_moveChilds);
	int count;
	ar(count);
	while (count-- != 0) {
		create()->deserialize(ar);
	}
}

void Group::serialize(OArchive& ar) const {
	ar(_moveChilds);
	ar(static_cast<int>(_childs.size()));
	for (auto& c : _childs) {
		c.first->serialize(ar);
	}
}
