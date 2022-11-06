#include "transmitter.hpp"

#include "broker.hpp"

using namespace al;

Transmitter::~Transmitter() {
	_broker.unsubscribeAll(this);
};

void Transmitter::subscribe(Transmitter* sender, type_id_t typeId, std::function<void(const void* data)> fn) {
	_broker.subscribe(this, sender, typeId,  std::move(fn));
}

void Transmitter::subscribe(type_id_t typeId, std::function<void(Transmitter* sender, const void* data)> fn) {
	_broker.subscribe(this, typeId, std::move(fn));
}

void Transmitter::subscribe(type_id_t typeId, std::function<void(const void* data)> fn) {
	_broker.subscribe(this, typeId, std::move(fn));
}

void Transmitter::unsubscribe(type_id_t typeId) {
	_broker.unsubscribe(this, typeId);
}

void Transmitter::unsubscribe(Transmitter* sender, type_id_t typeId) {
	_broker.unsubscribe(this, sender, typeId);
}

void Transmitter::unsubscribeAll() {
	_broker.unsubscribeAll(this);
}

void Transmitter::send(type_id_t typeId, const void* event) {
	_broker.send(this, typeId, event);
}
