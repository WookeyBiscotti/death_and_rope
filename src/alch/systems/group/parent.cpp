#include "parent.hpp"

#include "alch/engine/entity.hpp"
#include "group.hpp"

using namespace al;

Parent::Parent(Entity& entity): Component(entity) {
}

void Parent::parent(WeakPtr<Entity> parent) {
	removeFromParent();

	_parent = std::move(parent);

	auto p = _parent.lock();
	if (p) {
		if (auto gr = p->get<Group>()) {
			gr->add(entity().sharedFromThis());
		}
	}
}

void Parent::removeFromParent() {
	auto p = _parent.lock();
	if (p) {
		if (auto gr = p->get<Group>()) {
			gr->remove(&entity());
		}
	}
}
