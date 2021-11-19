#pragma once

#include <engine/component.hpp>
#include <engine/entity.hpp>
//
#include <memory>
#include <unordered_set>

class Group: public Component {
  public:
	struct SyncMove {};
	inline static const SyncMove SYNC_MOVE;

	explicit Group(Entity& entity): Component(entity) {}
	Group(Entity& entity, SyncMove);

	Entity& create() {
		auto e = std::make_unique<Entity>(entity().context());
		auto& ret = *e;
		add(std::move(e));

		return ret;
	}

	void add(std::unique_ptr<Entity> entity);
	void remove(const std::unique_ptr<Entity>& entity);

	const std::unordered_set<std::unique_ptr<Entity>>& childs() const { return _childs; }

  private:
	bool _moveChilds{};

	std::unordered_set<std::unique_ptr<Entity>> _childs;
};