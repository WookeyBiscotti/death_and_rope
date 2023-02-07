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

void Context::registerObject(const String& name, std::function<void(Context&)> creator) {
	auto found = _factory.find(name);
	if (found != _factory.end()) {
		LWARN("Factory for object name `{}` already exist", name);
		found->second = creator;

		return;
	}

	_factory[name] = creator;
}
