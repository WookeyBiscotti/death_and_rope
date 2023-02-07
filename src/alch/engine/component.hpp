#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/object.hpp"
#include "alch/common/prod_build_utils.hpp"
#include "alch/common/type_id.hpp"
#include "alch/common/type_id_utils.hpp"

#include <string_view>

namespace al {

class Entity;

class Component: public Object {
  public:
	explicit Component(Entity& entity):
	    _entity(entity){
	        // if constexpr (IS_NOT_PROD_BUILD) {
	        // 	for (const auto dp : dependsOn()) {
	        // 		assert(entity().get(dp));
	        // 	}
	        // }
	    };

	virtual ~Component() = default;

	Entity& entity() { return _entity; }

	virtual std::vector<type_id_t> dependsOn() const { return {}; }

  private:
	Entity& _entity;
};

#define AL_COMPONENT_DEPENDS_ON(...)                  \
	std::vector<type_id_t> dependsOn() const override { \
		return TypeIds<__VA_ARGS__>();                  \
	}

} // namespace al
