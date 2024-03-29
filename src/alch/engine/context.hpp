#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/inline_vector.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/prod_build_utils.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/type_id.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/logging/logger.hpp"
//
#include <functional>
#include <memory>

namespace sf {
class RenderTarget;
}

namespace al {

class Entity;
class Engine;

class Context final {
  public:
	Context();

	Context(const Context&) = delete;

	~Context();

	template<class S>
	S* addSystem(std::unique_ptr<S> s) {
		_systems.emplace(TypeId<S>(), s.get());
		_store.emplace_back(std::move(s));
	}

	template<class S, class... Args>
	S& createSystem(Args&&... args) {
		auto s = std::make_unique<S>(*this, std::forward<Args>(args)...);
		auto ret = s.get();
		_systems.emplace(TypeId<S>(), s.get());
		_store.emplace_back(std::move(s));

		return *ret;
	}

	template<class S>
	S* system() {
		if (const auto found = _systems.find(TypeId<S>()); found != _systems.end()) {
			return static_cast<S*>(found->second);
		}
		return nullptr;
	}

	template<class S>
	S& systemRef() {
		IF_NOT_PROD_BUILD(                              //
		    auto s = system<S>();                       //
		    if (!s) {                                   //
			    LERR("No such system {}", TypeId<S>()); //
			    assert(false);                          //
		    }                                           //
		    return *s;);
		return *static_cast<S*>(_systems.find(TypeId<S>())->second);
	}

	auto systems() const {
		std::vector<System*> systems;
		for (auto s : _systems) {
			systems.push_back(s.second);
		}
		return systems;
	}

	template<class T>
	void registerObject(const String& name, std::function<void(T&)> creator) {
		registerObject(name, typeId<T>,
		    [creator = std::move(creator)](void* userData) { creator(*static_cast<T*>(userData)); });
	}

  private:
	void registerObject(const String& name, type_id_t type, std::function<void(void*)> creator) {
		_factory[type][name] = creator;
	}

  private:
	FlatMap<type_id_t, System*> _systems;
	InlinedVector<std::unique_ptr<System>, 16> _store;
	FlatMap<type_id_t, FlatMap<String, std::function<void(void*)>>> _factory;
};

} // namespace al
