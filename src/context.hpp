#pragma once

#include <prod_build_utils.hpp>
#include <systems/logging/logger.hpp>
#include <type_id.hpp>
//
#include <functional>
#include <memory>
#include <unordered_map>

class Scene;
class Render;
class AssetCache;
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
			return static_cast<S*>(found.second);
		}
		return nullptr;
	}

	template<class S>
	S& systemRef() {
		NONPROD_ASSERT(_systems.contains(TypeId<S>()));
		return *static_cast<S*>(_systems[TypeId<S>()]);
	}

  private:
	std::unordered_map<type_id_t, void*> _systems;
};