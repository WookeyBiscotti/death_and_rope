#pragma once

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"


namespace sf {
class Event;
}

namespace al {

class Context;
class Entity;

class Scene: public Transmitter {
  public:
	virtual ~Scene() = default;

	explicit Scene(Context& context): Transmitter(context.systemRef<Broker>()), _context(context) {}

	virtual void onFrame(){};
	virtual void onEvent(const sf::Event& e){};

	Context& context() { return _context; }

	virtual void onStart() {}
	virtual void onStop() {}

	virtual Entity* root() { return nullptr; }

  private:
	Context& _context;
};

}
