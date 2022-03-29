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
class Context {
  public:
	template<class S>
	void addSystem(S* s) {
		_systems.emplace(TypeId<S>(), s);
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
};
