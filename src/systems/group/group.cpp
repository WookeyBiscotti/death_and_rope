#include "group.hpp"

#include <systems/transform/events.hpp>
#include <systems/transform/transform.hpp>

Group::Group(Entity& entity, SyncMove): Component(entity), _moveChilds(true) {
	entity.subscribe<PositionUpdate>(&entity, [this](const PositionUpdate& p) {
		for (auto& it : _childs) {
			it->ref<Transform>().p(it->ref<Transform>().p() + (p.neW - p.old));
		}
	});
}

void Group::add(std::unique_ptr<Entity> entity) {
	_childs.insert(std::move(entity));
}

void Group::remove(const std::unique_ptr<Entity>& entity) {
	_childs.erase(entity);
}