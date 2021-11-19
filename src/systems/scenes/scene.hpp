#pragma once

#include <engine/context.hpp>
#include <systems/broker/receiver.hpp>

class Context;

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

  private:
	Context& _context;
};