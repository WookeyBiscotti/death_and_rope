#pragma once

#include "alch/engine/component.hpp"
//
#include <memory>
#include <string_view>
#include <unordered_set>

class Entity;

class Group: public Component {
  public:
	struct SyncMove_t {};
	inline static const SyncMove_t SyncMove;

	explicit Group(Entity& entity): Component(entity) {}
	Group(Entity& entity, SyncMove_t);

	~Group();

	Entity& create();

	void add(std::unique_ptr<Entity> entity);
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

	std::vector<Entity*> _requestDelete;

	std::unordered_map<Entity*, std::unique_ptr<Entity>> _childs;
};
