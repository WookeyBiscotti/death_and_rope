#pragma once

#include "alch/common/type_id.hpp"

#include <functional>

namespace al {

class Broker;

class Transmitter {
  public:
	explicit Transmitter(Broker& broker): _broker(broker) {}
	virtual ~Transmitter();

	template<class Event>
	void send(const Event& event) {
		send(TypeId<Event>(), &event);
	}

	template<class Event>
	void subscribe(std::function<void(Transmitter* sender, const Event& event)> fn) {
		subscribe(TypeId<Event>(), [fn = std::move(fn)](Transmitter* sender, const void* data) {
			fn(sender, *reinterpret_cast<const Event*>(data));
		});
	}
	template<class Event>
	void subscribe(std::function<void(const Event& event)> fn) {
		subscribe(TypeId<Event>(),
		    [fn = std::move(fn)](const void* data) { fn(*reinterpret_cast<const Event*>(data)); });
	}
	template<class Event>
	void subscribe(Transmitter* sender, std::function<void(const Event& event)> fn) {
		subscribe(sender, TypeId<Event>(),
		    [fn = std::move(fn)](const void* data) { fn(*reinterpret_cast<const Event*>(data)); });
	}

	void subscribe(Transmitter* sender, type_id_t typeId, std::function<void(const void* data)> fn);
	void subscribe(type_id_t typeId, std::function<void(Transmitter* sender, const void* data)> fn);
	void subscribe(type_id_t typeId, std::function<void(const void* data)> fn);

	template<class Event>
	void unsubscribe() {
		unsubscribe(TypeId<Event>());
	}
	template<class Event>
	void unsubscribe(Transmitter* sender) {
		unsubscribe(sender, TypeId<Event>());
	}

	void unsubscribeAll();
	void unsubscribe(type_id_t typeId);
	void unsubscribe(Transmitter* sender, type_id_t typeId);

  private:
	void send(type_id_t typeId, const void* event);

  private:
	Broker& _broker;
};

} // namespace al
