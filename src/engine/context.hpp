#pragma once

#include <common/prod_build_utils.hpp>
#include <common/type_id.hpp>
#include <systems/logging/logger.hpp>
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

  private:
	std::unordered_map<type_id_t, void*> _systems;
};