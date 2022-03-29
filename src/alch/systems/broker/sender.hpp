#pragma once

#include "alch/common/type_id.hpp"

class Broker;

class Sender {
  public:
	explicit Sender(Broker& broker): _broker(broker) {}
	virtual ~Sender();

	template<class Event>
	void send(const Event& event) {
		send(TypeId<Event>(), &event);
	}

  private:
	void send(type_id_t typeId, const void* event);

  private:
	Broker& _broker;
};
