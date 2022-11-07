#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/engine/component.hpp"
//
#include <string_view>

namespace al {

class Entity;

class Parent: public Component {
  public:
	explicit Parent(Entity& entity);

	void parent(WeakPtr<Entity> parent);
	WeakPtr<Entity> parent() const { return _parent; }

	void removeFromParent();

	void serialize(OArchive& ar) const override {}
	void deserialize(IArchive& ar) override {}

	ALCH_COMPONENT_NAME(Parent);

  private:
	WeakPtr<Entity> _parent;
};

} // namespace al