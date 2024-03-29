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
	AL_OBJECT_REGISTER();

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

	void save(OArchive& archive) const override;
	void load(IArchive& archive) override;

  private:
	bool _moveChilds{};

	FlatMap<Entity*, SharedPtr<Entity>> _childs;
};

} // namespace al
