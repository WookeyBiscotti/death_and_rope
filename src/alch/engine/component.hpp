#pragma once

#include "alch/common/archive.hpp"

class Entity;
// class IArchive;
// class OArchive;

class Component {
  public:
	explicit Component(Entity& entity): _entity(entity){};

	virtual ~Component() = default;

	Entity& entity() { return _entity; }

	virtual void serialize(OArchive& archive) const = 0;
	virtual void deserialize(IArchive& archive) = 0;

  private:
	Entity& _entity;
};
