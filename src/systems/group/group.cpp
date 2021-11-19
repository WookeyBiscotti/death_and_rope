#include "group.hpp"

#include <systems/position/events.hpp>
#include <systems/position/position.hpp>

Group::Group(Entity& entity, SyncMove): Component(entity), _moveChilds(true) {
	entity.subscribe<PositionUpdate>(&entity, [this](const PositionUpdate& p) {
		for (auto& it : _childs) {
			it->ref<Position>()(it->ref<Position>()() + (p.neW - p.old));
		}
	});
}

void Group::add(std::unique_ptr<Entity> entity) {
	_childs.insert(std::move(entity));
}

void Group::remove(const std::unique_ptr<Entity>& entity) {
	_childs.erase(entity);
}