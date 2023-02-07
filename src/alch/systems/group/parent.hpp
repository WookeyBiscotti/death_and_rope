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
	AL_OBJECT_REGISTER();

	explicit Parent(Entity& entity);

	void parent(WeakPtr<Entity> parent);
	WeakPtr<Entity> parent() const { return _parent; }

	void removeFromParent();

	void save(OArchive& archive) const override {}
	void load(IArchive& archive) override {}

  private:
	WeakPtr<Entity> _parent;
};

} // namespace al
