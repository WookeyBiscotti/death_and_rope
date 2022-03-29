#include "broker.hpp"
#include "sender.hpp"

void Sender::send(type_id_t typeId, const void* event) {
	_broker.send(this, typeId, event);
}

Sender::~Sender() {
	_broker.unsubscribeAll(this);
}