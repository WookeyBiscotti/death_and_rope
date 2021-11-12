#pragma once

class Entity;

class Component {
  public:
	explicit Component(Entity& entity): _entity(entity){};

	virtual ~Component() = default;

	Entity& entity() { return _entity; }

	virtual void removeFromSystem() {}

  private:
	Entity& _entity;
};