#pragma once

#include <engine/context.hpp>
#include <systems/broker/broker.hpp>

class Context;
class Entity;

namespace sf {
struct Event;
}

class Scene: public Receiver {
  public:
	virtual ~Scene() = default;

	explicit Scene(Context& context): Receiver(context.systemRef<Broker>()), _context(context) {}

	virtual void onFrame(){};
	virtual void onEvent(const sf::Event& e){};

	Context& context() { return _context; }

	virtual bool active() const { return true; };
	virtual void active(bool active){};

	virtual Entity* root() { return nullptr; }

  private:
	Context& _context;
};