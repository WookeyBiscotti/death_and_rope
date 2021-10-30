#include "receiver.hpp"

#include "broker.hpp"

Receiver::~Receiver() {
	_broker.unsubscribeAll(this);
};

void Receiver::subscribe(type_id_t typeId, std::function<void(Sender* sender, const void* data)> fn) {
	_broker.subscribe(this, typeId, std::move(fn));
}

void Receiver::subscribe(type_id_t typeId, std::function<void(const void* data)> fn) {
	_broker.subscribe(this, typeId, std::move(fn));
}
