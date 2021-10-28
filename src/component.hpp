#pragma once

class Entity;

class Component
{
public:
    Component(Entity& entity)
        : _entity(entity){};

    virtual ~Component() = default;

    Entity& entity() { return _entity; }

private:
    Entity& _entity;
};