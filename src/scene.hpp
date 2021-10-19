#pragma once

class Context;

namespace sf {
struct Event;
}

class Scene
{
public:
    virtual ~Scene() = default;

    Scene(Context& context)
        : _context(context)
    {}

    virtual void onFrame() = 0;
    virtual void onEvent(const sf::Event& e){};

    Context& context() { return _context; }

    virtual bool active() const = 0;
    virtual void active(bool active) = 0;

private:
    Context& _context;
};