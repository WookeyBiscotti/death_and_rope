#pragma once

#include "receiver.hpp"
#include "sender.hpp"
//
#include "alch/common/map_utils.hpp"
#include "alch/common/type_id.hpp"
#include "alch/engine/system.hpp"
//
#include <unordered_map>
#include <unordered_set>

class Broker: public System {
  public:
	template<class Event>
	void subscribe(Receiver* receiver, std::function<void(Sender* sender, const Event& event)> fn) {
		_eventsFn[TypeId<Event>()] = {TypeId<Event>(),
		    [fn = std::move(fn)](Sender* sender, const void* data) { fn(sender, reinterpret_cast<Event*>(data)); }};
		_receiversFns[receiver].emplace(TypeId<Event>());
	}

	template<class Event>
	void subscribe(Receiver* receiver, std::function<void(const Event& event)> fn) {
		_eventsFn[TypeId<Event>()][receiver] = EventFromAllListner{TypeId<Event>(),
		    [fn = std::move(fn)](Sender* sender, const void* data) { fn(*reinterpret_cast<const Event*>(data)); }};
		_receiversFns[receiver].emplace(TypeId<Event>());
	}

	void subscribe(Receiver* receiver, type_id_t typeId, std::function<void(const void* event)> fn) {
		_eventsFn[typeId][receiver] =
		    EventFromAllListner{typeId, [fn = std::move(fn)](Sender* sender, const void* data) { fn(data); }};
		_receiversFns[receiver].emplace(typeId);
	}

	void subscribe(Receiver* receiver, type_id_t typeId, std::function<void(Sender* sender, const void* event)> fn) {
		_eventsFn[typeId][receiver] = EventFromAllListner{typeId, std::move(fn)};
		_receiversFns[receiver].emplace(typeId);
	}

	template<class Event>
	void unsubscribe(Receiver* receiver) {
		unsubscribe(receiver, TypeId<Event>());
	}

	void unsubscribe(Receiver* receiver, type_id_t typeId) {
		if (auto types = findValue(_receiversFns, receiver); types) {
			types->erase(typeId);
		}
		if (auto receivers = findValue(_eventsFn, typeId); receivers) {
			receivers->erase(receiver);
		}
	}

	void unsubscribeAll(Receiver* receiver) {
		if (auto types = findValue(_receiversFns, receiver); types) {
			while (!types->empty()) {
				unsubscribe(receiver, *types->begin());
			}
		}
	}

	template<class Event>
	void send(Sender* sender, const Event& event) {
		send(sender, TypeId<Event>(), &event);
	}
	template<class Event>
	void send(const Event& event) {
		send(nullptr, event);
	}

	void send(Sender* sender, type_id_t typeId, const void* data) {
		//TODO: use small vector
		std::vector<std::function<void(Sender * sender, const void* data)>> allFnStack;
		std::vector<std::function<void(const void* data)>> oneFnStack;

		if (auto receivers = findValue(_eventsFn, typeId); receivers) {
			for (auto r : *receivers) {
				allFnStack.push_back(r.second.fn);
			}
		}

		if (sender) {
			if (const auto& types = findValue(_personalEventsFn, sender); types) {
				if (const auto& receivers = findValue(*types, typeId); receivers) {
					for (const auto& r : *receivers) {
						oneFnStack.push_back(r.second.fn);
					}
				}
			}
		}

		while (!allFnStack.empty()) {
			auto fn = std::move(allFnStack.back());
			allFnStack.pop_back();
			fn(sender, data);
		}
		while (!oneFnStack.empty()) {
			auto fn = std::move(oneFnStack.back());
			oneFnStack.pop_back();
			fn(data);
		}
	}

	void subscribe(Receiver* r, Sender* s, type_id_t typeId, std::function<void(const void* event)> fn) {
		_personalEventsFn[s][typeId][r] = EventFromOneListner{typeId, std::move(fn)};
		_personalReceiversFns[r][s].insert(typeId);
	}

	void unsubscribe(Receiver* receiver, Sender* sender, type_id_t typeId) {
		_personalEventsFn[sender][typeId].erase(receiver);
		_personalReceiversFns[receiver][sender].erase(typeId);
	}

	void unsubscribe(Receiver* receiver, Sender* sender) {
		if (auto senders = findValue(_personalReceiversFns, receiver)) {
			senders->erase(sender);
		}
		_personalEventsFn[sender].erase(receiver);
	}

	void unsubscribeAll(Sender* sender) {
		if (auto types = findValue(_personalEventsFn, sender)) {
			for (const auto& type : *types) {
				for (const auto& receiver : type.second) {
					if (auto senders = findValue(_personalReceiversFns, receiver.first)) {
						senders->erase(sender);
					}
				}
			}
		}
		_personalEventsFn.erase(sender);
	}

  private:
	struct EventFromAllListner {
		type_id_t typeId;
		std::function<void(Sender* sender, const void* data)> fn;
	};

	struct EventFromOneListner {
		type_id_t typeId;
		std::function<void(const void* data)> fn;
	};

	std::unordered_map<type_id_t, std::unordered_map<Receiver*, EventFromAllListner>> _eventsFn;
	std::unordered_map<Receiver*, std::unordered_set<type_id_t>> _receiversFns;

	std::unordered_map<Sender*, std::unordered_map<type_id_t, std::unordered_map<Receiver*, EventFromOneListner>>>
	    _personalEventsFn;
	std::unordered_map<Receiver*, std::unordered_map<Sender*, std::unordered_set<type_id_t>>> _personalReceiversFns;
};
