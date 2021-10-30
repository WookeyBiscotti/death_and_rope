#include "sender.hpp"

#include "broker.hpp"

void Sender::send(type_id_t typeId, const void* event) {
	_broker.send(this, typeId, event);
}