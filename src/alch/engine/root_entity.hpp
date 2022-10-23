#pragma once

#include "entity.hpp"

namespace al {

class RootEntity: public Entity {
  public:
	explicit RootEntity(Context& context): Entity(context) {}

	void pushToDeleteList(SharedPtr<Entity> e);

	void releaseDeleteList();

  private:
	SharedPtr<Entity> _waitForDelete;
};

} // namespace al
