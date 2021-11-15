#pragma once

#include "receiver.hpp"
#include "sender.hpp"
//
#include <common/map_utils.hpp>
#include <common/type_id.hpp>
//
#include <unordered_map>
#include <unordered_set>

class Broker {
  public:
	template<class Event>
	void subscribe(Receiver* receiver, std::function<void(Sender* sender, const Event& event)> fn) {
		_eventsFn.emplace(TypeId<Event>(), {TypeId<Event>(), [fn = std::move(fn)](Sender* sender, const void* data) {
			                                    fn(sender, reinterpret_cast<Event*>(data));
		                                    }});

		_receiversFns[receiver].emplace(TypeId<Event>());
	}
	template<class Event>
	void subscribe(Receiver* receiver, std::function<void(const Event& event)> fn) {
		_eventsFn[TypeId<Event>()].emplace(receiver,
		    EventFromAllListner{TypeId<Event>(),
		        [fn = std::move(fn)](Sender* sender, const void* data) { fn(*reinterpret_cast<const Event*>(data)); }});

		_receiversFns[receiver].emplace(TypeId<Event>());
	}

	void subscribe(Receiver* receiver, type_id_t typeId, std::function<void(Sender* sender, const void* event)> fn) {
		_eventsFn[typeId].emplace(receiver, EventFromAllListner{typeId, std::move(fn)});
		_receiversFns[receiver].emplace(typeId);
	}
	void subscribe(Receiver* receiver, type_id_t typeId, std::function<void(const void* event)> fn) {
		_eventsFn[typeId].emplace(receiver,
		    EventFromAllListner{typeId, [fn = std::move(fn)](Sender* sender, const void* data) { fn(data); }});
		_receiversFns[receiver].emplace(typeId);
	}

	template<class Event>
	void unsubscribe(Receiver* receiver) {
		unsubscribe(receiver, TypeId<Event>());
	}
	void unsubscribeAll(Receiver* receiver) {
		ifExist(_receiversFns, receiver, [this, receiver](auto&& c) { //
			while (!c.empty()) {
				unsubscribe(receiver, *c.begin());
			}
		});
	}
	void unsubscribe(Receiver* receiver, type_id_t typeId) {
		ifExist(_eventsFn, typeId, [this, receiver](auto&& c) { //
			c.erase(receiver);
		});
		ifExist(_receiversFns, receiver, [this, typeId, receiver](auto&& c) { //
			c.erase(typeId);
			if (c.empty()) {
				_receiversFns.erase(receiver);
			}
		});
	}

	template<class Event>
	void send(Sender* sender, const Event& event) {
		ifExist(_eventsFn, TypeId<Event>(), [sender, &event](auto&& c) { //
			for (const auto& r : c) {
				r.second.fn(sender, &event);
			}
		});
	}
	template<class Event>
	void send(const Event& event) {
		send(nullptr, event);
	}

	void send(Sender* sender, type_id_t typeId, const void* data) {
		ifExist(_eventsFn, typeId, [sender, data](auto&& c) { //
			for (const auto& r : c) {
				r.second.fn(sender, data);
			}
		});
	}

  private:
	struct EventFromAllListner {
		type_id_t typeId;
		std::function<void(Sender* sender, const void* data)> fn;
	};

	// struct EventListner {
	// 	Receiver* receiver;
	// 	Sender* sender;
	// 	type_id_t typeId;
	// 	std::function<void(Sender* sender, void* data)> fn;
	// };

	std::unordered_map<type_id_t, std::unordered_map<Receiver*, EventFromAllListner>> _eventsFn;
	std::unordered_map<Receiver*, std::unordered_set<type_id_t>> _receiversFns;
};
