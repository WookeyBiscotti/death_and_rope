#pragma once
#include "transmitter.hpp"
//
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/hash_set.hpp"
#include "alch/common/containers/inline_vector.hpp"
#include "alch/common/map_utils.hpp"
#include "alch/common/type_id.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/logging/logger.hpp"
//
//
#include <functional>
#include <vector>

namespace al {

class Broker: public System {
  public:
	Broker(Context& context);

	template<class Event>
	void subscribe(Transmitter* receiver, std::function<void(Transmitter* sender, const Event& event)>&& fn) {
		addNewListner(receiver, {TypeId<Event>(), [fn = std::move(fn)](Transmitter* sender, const void* data) {
			                         fn(sender, reinterpret_cast<Event*>(data));
		                         }});
	}

	void subscribe(Transmitter* receiver, type_id_t typeId,
	    std::function<void(Transmitter* sender, const void* event)>&& fn) {
		addNewListner(receiver, {typeId, std::move(fn)});
	}

	template<class Event>
	void subscribe(Transmitter* receiver, std::function<void(const Event& event)>&& fn) {
		addNewListner(receiver, {TypeId<Event>(), [fn = std::move(fn)](Transmitter* sender, const void* data) {
			                         fn(*reinterpret_cast<const Event*>(data));
		                         }});
	}

	void subscribe(Transmitter* receiver, type_id_t typeId, std::function<void(const void* event)>&& fn) {
		addNewListner(receiver, {typeId, [fn = std::move(fn)](Transmitter* sender, const void* data) { fn(data); }});
	}

	void subscribe(Transmitter* r, Transmitter* s, type_id_t typeId, std::function<void(const void* event)>&& fn) {
		addNewPersonalListner(r, s, EventFromOneListner{typeId, std::move(fn)});
	}

	template<class Event>
	void send(Transmitter* sender, const Event& event) {
		send(sender, TypeId<Event>(), &event);
	}
	template<class Event>
	void send(const Event& event) {
		send(nullptr, event);
	}

	void send(Transmitter* sender, type_id_t typeId, const void* data) {
		_currentCallDeep++;

		if (auto receivers = findValue(_eventsFn, typeId); receivers) {
			for (auto r : *receivers) {
				r.second.fn(sender, data);
			}
		}

		if (sender) {
			if (const auto& types = findValue(_personalEventsFn, sender); types) {
				if (const auto& receivers = findValue(*types, typeId); receivers) {
					for (const auto& r : *receivers) {
						r.second.fn(data);
					}
				}
			}
		}

		_currentCallDeep--;

		if (_currentCallDeep == 0 && _dirtyFlag) {
			for (const auto& r : _deleteRWait) {
				unsubscribeAll(r);
			}
			_deleteRWait.clear();
			for (const auto& [r, s] : _deleteRSWait) {
				unsubscribe(r, s);
			}
			_deleteRSWait.clear();
			for (const auto& [r, t] : _deleteRTWait) {
				unsubscribe(r, t);
			}
			_deleteRTWait.clear();
			for (const auto& [r, s, t] : _deleteRSTWait) {
				unsubscribe(r, s, t);
			}
			_deleteRSTWait.clear();

			for (auto&& [r, s, l] : _addPersonalWait) {
				addNewPersonalListnerImpl(r, s, std::move(l));
			}
			_addPersonalWait.clear();
			for (auto&& [r, l] : _addWait) {
				addNewListnerImpl(r, std::move(l));
			}
			_addWait.clear();

			_dirtyFlag = false;
		}
	}

	template<class Event>
	void unsubscribe(Transmitter* receiver) {
		unsubscribeImpl(receiver, TypeId<Event>());
	}

	void unsubscribe(Transmitter* receiver, type_id_t typeId) {
		if (_currentCallDeep != 0) {
			_deleteRTWait.emplace_back(receiver, typeId);
			_dirtyFlag = true;

			return;
		}

		unsubscribeImpl(receiver, typeId);
	}

	void unsubscribe(Transmitter* receiver, Transmitter* sender, type_id_t typeId) {
		if (_currentCallDeep != 0) {
			_deleteRSTWait.emplace_back(receiver, sender, typeId);
			_dirtyFlag = true;

			return;
		}

		unsubscribeImpl(receiver, sender, typeId);
	}

	void unsubscribe(Transmitter* receiver, Transmitter* sender) {
		if (_currentCallDeep != 0) {
			_deleteRSWait.emplace_back(receiver, sender);
			_dirtyFlag = true;

			return;
		}

		unsubscribeImpl(receiver, sender);
	}

	void unsubscribeAll(Transmitter* receiver) {
		if (_currentCallDeep != 0) {
			_deleteRWait.emplace_back(receiver);
			_dirtyFlag = true;

			return;
		}

		unsubscribeAllImpl(receiver);
	}

  private:
	struct EventFromAllListner {
		type_id_t typeId;
		std::function<void(Transmitter* sender, const void* data)> fn;
		bool deleted = false;
	};

	struct EventFromOneListner {
		type_id_t typeId;
		std::function<void(const void* data)> fn;
		bool deleted = false;
	};

	int _currentCallDeep = 0;

	bool _dirtyFlag = false;

	InlinedVector<Transmitter*, 64> _deleteRWait;
	InlinedVector<std::tuple<Transmitter* /*r*/, type_id_t>, 64> _deleteRTWait;
	InlinedVector<std::tuple<Transmitter* /*r*/, Transmitter* /*s*/>, 64> _deleteRSWait;
	InlinedVector<std::tuple<Transmitter* /*r*/, Transmitter* /*s*/, type_id_t>, 64> _deleteRSTWait;

	InlinedVector<std::tuple<Transmitter*, EventFromAllListner>, 64> _addWait;
	InlinedVector<std::tuple<Transmitter*, Transmitter*, EventFromOneListner>, 64> _addPersonalWait;

	void addNewListner(Transmitter* r, EventFromAllListner&& l) {
		if (_currentCallDeep != 0) {
			_addWait.emplace_back(r, std::move(l));
			_dirtyFlag = true;

			return;
		}

		addNewListnerImpl(r, std::move(l));
	}

	void addNewPersonalListner(Transmitter* r, Transmitter* s, EventFromOneListner&& l) {
		if (_currentCallDeep != 0) {
			_addPersonalWait.emplace_back(r, s, std::move(l));
			_dirtyFlag = true;

			return;
		}

		addNewPersonalListnerImpl(r, s, std::move(l));
	}

	void addNewListnerImpl(Transmitter* r, EventFromAllListner&& l) {
		_receiversFns[r].emplace(l.typeId);
		_eventsFn[l.typeId][r] = std::move(l);
	}

	void addNewPersonalListnerImpl(Transmitter* r, Transmitter* s, EventFromOneListner&& l) {
		_personalReceiversFns[r][s].insert(l.typeId);
		_personalEventsFn[s][l.typeId][r] = std::move(l);
	}

	void unsubscribeImpl(Transmitter* receiver, type_id_t typeId) {
		if (auto types = findValue(_receiversFns, receiver); types) {
			types->erase(typeId);
		}
		if (auto receivers = findValue(_eventsFn, typeId); receivers) {
			receivers->erase(receiver);
		}
	}

	void unsubscribeImpl(Transmitter* receiver, Transmitter* sender, type_id_t typeId) {
		_personalEventsFn[sender][typeId].erase(receiver);
		_personalReceiversFns[receiver][sender].erase(typeId);
	}

	void unsubscribeImpl(Transmitter* receiver, Transmitter* sender) {
		if (auto senders = findValue(_personalReceiversFns, receiver)) {
			senders->erase(sender);
		}
		_personalEventsFn[sender].erase(receiver);
	}

	void unsubscribeAllImpl(Transmitter* trans) {

		// remove as sender
		if (auto types = findValue(_personalEventsFn, trans)) {
			for (const auto& type : *types) {
				for (const auto& receiver : type.second) {
					if (auto senders = findValue(_personalReceiversFns, receiver.first)) {
						senders->erase(trans);
					}
				}
			}
		}
		_personalEventsFn.erase(trans);

		// remove as receiver
		if (auto types = findValue(_receiversFns, trans); types) {
			while (!types->empty()) {
				unsubscribe(trans, *types->begin());
			}
		}
	}

	FlatMap<type_id_t, FlatMap<Transmitter*, EventFromAllListner>> _eventsFn;
	FlatMap<Transmitter*, FlatSet<type_id_t>> _receiversFns;

	FlatMap<Transmitter*, FlatMap<type_id_t, FlatMap<Transmitter*, EventFromOneListner>>> _personalEventsFn;
	FlatMap<Transmitter*, FlatMap<Transmitter*, FlatSet<type_id_t>>> _personalReceiversFns;
};

} // namespace al
