#pragma once

#include "alch/common/archive.hpp"

#include <string_view>

class Entity;

class Component {
  public:
	explicit Component(Entity& entity): _entity(entity){};

	virtual ~Component() = default;

	Entity& entity() { return _entity; }

	virtual std::string_view cName() const = 0;

	virtual void serialize(OArchive& archive) const = 0;
	virtual void deserialize(IArchive& archive) = 0;

  private:
	Entity& _entity;
};

#define ALCH_COMPONENT_NAME(NAME)                   \
	std::string_view cName() const override {       \
		static const std::string_view name = #NAME; \
		return name;                                \
	};
