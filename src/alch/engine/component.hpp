#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/prod_build_utils.hpp"
#include "alch/common/type_id.hpp"
#include "alch/common/type_id_utils.hpp"

#include <string_view>

namespace al {

class Entity;

class Component {
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

	virtual std::string_view cName() const = 0;

	virtual void save(VarOArchive& archive) const = 0;
	virtual void load(VarIArchive& archive) = 0;

	virtual std::vector<type_id_t> dependsOn() const { return {}; }

  private:
	Entity& _entity;
};

#define ALCH_COMPONENT_NAME(NAME)                   \
	std::string_view cName() const override {       \
		static const std::string_view name = #NAME; \
		return name;                                \
	};

#define ALCH_COMPONENT_DEPENDS_ON(...)                  \
	std::vector<type_id_t> dependsOn() const override { \
		return TypeIds<__VA_ARGS__>();                  \
	}

} // namespace al
