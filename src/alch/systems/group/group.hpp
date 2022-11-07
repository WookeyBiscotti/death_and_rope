#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/engine/component.hpp"
#include "alch/systems/broker/transmitter.hpp"

//
#include <memory>
#include <string_view>
#include <unordered_set>

namespace al {

class Entity;

class Group: public Component, public Transmitter {
  public:
	struct SyncMove_t {};
	inline static const SyncMove_t SyncMove;

	explicit Group(Entity& entity);
	Group(Entity& entity, SyncMove_t);

	SharedPtr<Entity> create();

	void add(SharedPtr<Entity> entity);
	void remove(Entity* entity);

	bool moveChilds() const { return _moveChilds; }
	void moveChilds(bool moveChilds) { _moveChilds = moveChilds; }

	std::vector<Entity*> childs() const {
		std::vector<Entity*> childs;
		childs.reserve(_childs.size());
		for (const auto& c : _childs) {
			childs.push_back(c.first);
		}

		return childs;
	}

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

	ALCH_COMPONENT_NAME(Group);

  private:
	bool _moveChilds{};

	HashMap<Entity*, SharedPtr<Entity>> _childs;
};

} // namespace al
