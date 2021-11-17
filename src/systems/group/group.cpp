#include "group.hpp"

#include <systems/broker/receiver_component.hpp>
#include <systems/broker/sender_component.hpp>
#include <systems/position/events.hpp>
#include <systems/position/position.hpp>

Group::Group(Entity& entity, bool moveChilds): Component(entity), _moveChilds(moveChilds) {
	if (_moveChilds) {
		auto r = entity.get<ReceiverComponent>();
		auto s = entity.get<SenderComponent>();
		if (r && s) {
			r->subscribe<PositionUpdate>(s, [this](const PositionUpdate& p) {
				for (auto& it : _childs) {
					it->ref<Position>()(it->ref<Position>()() + (p.neW - p.old));
				}
			});
		}
	}
}

void Group::add(std::unique_ptr<Entity> entity) {
	_childs.insert(std::move(entity));
}

void Group::remove(const std::unique_ptr<Entity>& entity) {
	_childs.erase(entity);
}