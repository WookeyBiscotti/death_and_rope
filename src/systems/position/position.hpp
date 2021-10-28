#pragma once

#include <component.hpp>
#include <vector2.hpp>

class Position : public Component
{
public:
    Position(Entity& entity)
        : Component(entity){};

    const Vector2f& position() const { return _position; }
    void position(const Vector2f& position) { _position = position; }

private:
    Vector2f _position{};
};