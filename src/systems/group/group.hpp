#pragma once

#include <engine/component.hpp>
//
#include <memory>
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
	void remove(const std::unique_ptr<Entity>& entity);

	const std::unordered_set<std::unique_ptr<Entity>>& childs() const { return _childs; }

	template<class Archive>
	void serialize(Archive& ar);

  private:
	bool _moveChilds{};

	std::unordered_set<std::unique_ptr<Entity>> _childs;
};