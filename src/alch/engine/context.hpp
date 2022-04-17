#pragma once

#include "alch/common/prod_build_utils.hpp"
#include "alch/common/type_id.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/logging/logger.hpp"
//
#include <functional>
#include <memory>
#include <unordered_map>

namespace sf {
class RenderTarget;
}

class Engine;

class Context final {
  public:
	Engine& engine;

	Context(Engine& engine): engine(engine) {}

	~Context() {
		while (!_store.empty()) {
			_store.pop_back();
		}
		_systems.clear(); 
	}

	template<class S>
	S* addSystem(std::unique_ptr<S> s) {
		_systems.emplace(TypeId<S>(), s.get());
		_store.emplace_back(std::move(s));
	}

	template<class S, class... Args>
	S& createSystem(Args&&... args) {
		auto s = std::make_unique<S>(std::forward<Args>(args)...);
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

  private:
	std::unordered_map<type_id_t, System*> _systems;
	std::vector<std::unique_ptr<System>> _store;
};
