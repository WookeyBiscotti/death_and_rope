#pragma once

#include <common/type_id.hpp>
//
#include <functional>

class Broker;
class Sender;

class Receiver {
  public:
	explicit Receiver(Broker& broker): _broker(broker) {}
	virtual ~Receiver();

	template<class Event>
	void subscribe(std::function<void(Sender* sender, const Event& event)> fn) {
		subscribe(TypeId<Event>(), [fn = std::move(fn)](Sender* sender, const void* data) {
			fn(sender, *reinterpret_cast<const Event*>(data));
		});
	}
	template<class Event>
	void subscribe(std::function<void(const Event& event)> fn) {
		subscribe(TypeId<Event>(),
		    [fn = std::move(fn)](const void* data) { fn(*reinterpret_cast<const Event*>(data)); });
	}
	template<class Event>
	void subscribe(Sender* sender, void* CbId, std::function<void(const Event& event)> fn) {
		subscribe(sender, TypeId<Event>(), CbId,
		    [fn = std::move(fn)](const void* data) { fn(*reinterpret_cast<const Event*>(data)); });
	}

	void subscribe(Sender* sender, type_id_t typeId, void* cbId, std::function<void(const void* data)> fn);
	void subscribe(type_id_t typeId, std::function<void(Sender* sender, const void* data)> fn);
	void subscribe(type_id_t typeId, std::function<void(const void* data)> fn);

	template<class Event>
	void unsubscribe() {
		unsubscribe(TypeId<Event>());
	}
	template<class Event>
	void unsubscribe(Sender* sender, void* CbId) {
		unsubscribe(sender, TypeId<Event>(), CbId);
	}

	void unsubscribeAll();
	void unsubscribe(type_id_t typeId);
	void unsubscribe(Sender* sender, type_id_t typeId, void* CbId);

  private:
	Broker& _broker;
};