#include "context.hpp"

#include "alch/engine/entity.hpp"
#include "alch/systems/broker/broker.hpp"

using namespace al;

Context::Context() {
	createSystem<Broker>();
}

Context::~Context() {
	while (!_store.empty()) {
		_store.pop_back();
	}

	_systems.clear();
}

void Context::pushToDeleteList(SharedPtr<Entity> e) {
	e->_nextForDelete = _waitForDelete;
	_waitForDelete = std::move(e);
}

void Context::releaseDeleteList() {
	auto e = std::move(_waitForDelete);
	while (e) {
		if (!e.unique()) {
			removeOnTickEnd(e);
		}
		e = std::move(e->_nextForDelete);
	}
}
