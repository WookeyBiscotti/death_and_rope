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
