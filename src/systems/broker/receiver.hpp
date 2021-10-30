#pragma once

#include <type_id.hpp>
//
#include <functional>

class Broker;
class Sender;

class Receiver {
  public:
	Receiver(Broker& broker): _broker(broker) {}
	virtual ~Receiver();

	template<class Event>
	void subscribe(std::function<void(Sender* sender, const Event& event)> fn) {
		subscribe(typeId<Event>(), [fn = std::move(fn)](Sender* sender, const void* data) {
			fn(sender, *reinterpret_cast<const Event*>(data));
		});
	}
	template<class Event>
	void subscribe(std::function<void(const Event& event)> fn) {
		subscribe(
		    typeId<Event>(), [fn = std::move(fn)](const void* data) { fn(*reinterpret_cast<const Event*>(data)); });
	}

  private:
	void subscribe(type_id_t typeId, std::function<void(Sender* sender, const void* data)> fn);
	void subscribe(type_id_t typeId, std::function<void(const void* data)> fn);

  private:
	Broker& _broker;
};