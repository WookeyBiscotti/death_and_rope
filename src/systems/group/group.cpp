#include "group.hpp"

#include <engine/entity.hpp>
#include <systems/transform/events.hpp>
#include <systems/transform/transform.hpp>

Group::Group(Entity& entity, SyncMove_t): Component(entity), _moveChilds(true) {
	entity.subscribe<PositionUpdate>(&entity, this, [this](const PositionUpdate& p) {
		// TODO: use subscription
		if (!_moveChilds) {
			return;
		}
		for (auto& it : _childs) {
			it->ref<Transform>().p(it->ref<Transform>().p() + (p.neW - p.old));
		}
	});
}

Group::~Group() {
	entity().unsubscribe<PositionUpdate>(&entity(), this);
}

Entity& Group::create() {
	auto e = std::make_unique<Entity>(entity().context());
	auto& ret = *e;
	add(std::move(e));

	return ret;
}

void Group::add(std::unique_ptr<Entity> entity) {
	_childs.insert(std::move(entity));
}

void Group::remove(const std::unique_ptr<Entity>& entity) {
	_childs.erase(entity);
}

template<>
void Group::serialize(IArchive& ar) {
	ar(_moveChilds);
	int count;
	ar(count);
	while (count-- != 0) {
		create().serialize(ar);
	}
}

template<>
void Group::serialize(OArchive& ar) {
	ar(_moveChilds);
	ar(static_cast<int>(_childs.size()));
	for (auto& c : _childs) {
		c->serialize(ar);
	}
}