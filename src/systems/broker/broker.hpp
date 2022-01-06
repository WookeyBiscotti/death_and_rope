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

	void subscribe(Receiver* receiver, type_id_t typeId, std::function<void(const void* event)> fn) {
		_eventsFn[typeId].emplace(receiver,
		    EventFromAllListner{typeId, [fn = std::move(fn)](Sender* sender, const void* data) { fn(data); }});
		_receiversFns[receiver].emplace(typeId);
	}

	void subscribe(Receiver* receiver, type_id_t typeId, std::function<void(Sender* sender, const void* event)> fn) {
		_eventsFn[typeId].emplace(receiver, EventFromAllListner{typeId, std::move(fn)});
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
		if (auto receivers = findValue(_eventsFn, typeId); receivers) {
			for (auto r : *receivers) {
				r.second.fn(sender, data);
			}
		}

		if (sender) {
			if (const auto& types = findValue(_personalEventsFn, sender); types) {
				if (const auto& receivers = findValue(*types, typeId); receivers) {
					for (const auto& r : *receivers) {
						for (const auto& c : r.second) {
							c.second.fn(data);
						}
					}
				}
			}
		}
	}

	void subscribe(Receiver* r, Sender* s, type_id_t typeId, void* cbId, std::function<void(const void* event)> fn) {
		_personalEventsFn[s][typeId][r][cbId] = EventFromOneListner{typeId, std::move(fn)};
		_personalReceiversFns[r][s].insert(typeId);
	}

	void unsubscribe(Receiver* receiver, Sender* sender, type_id_t typeId, void* cbId) {
		if (auto types = findValue(_personalEventsFn, sender)) {
			if (auto receivers = findValue(*types, typeId)) {
				if (auto cbs = findValue(*receivers, receiver)) {
					cbs->erase(cbId);
					if (cbs->empty()) {
						if (auto senders = findValue(_personalReceiversFns, receiver)) {
							if (auto types = findValue(*senders, sender)) {
								types->erase(typeId);
							}
						}
						receivers->erase(receiver);
					}
					if (receivers->empty()) {
						types->erase(typeId);
					}
					if (types->empty()) {
						_personalEventsFn.erase(sender);
					}
				}
			}
		}
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

	std::unordered_map<Sender*,
	    std::unordered_map<type_id_t, std::unordered_map<Receiver*, std::unordered_map<void*, EventFromOneListner>>>>
	    _personalEventsFn;
	std::unordered_map<Receiver*, std::unordered_map<Sender*, std::unordered_set<type_id_t>>> _personalReceiversFns;
};
