#include "root_entity.hpp"

using namespace al;

void RootEntity::pushToDeleteList(SharedPtr<Entity> e) {
	e->_nextForDelete = _waitForDelete;
	_waitForDelete = std::move(e);
}

void RootEntity::releaseDeleteList() {
	auto e = std::move(_waitForDelete);
	while (e) {
		if (!e.unique()) {
			pushToDeleteList(e);
		}
		e = std::move(e->_nextForDelete);
	}
}
