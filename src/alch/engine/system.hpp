#pragma once

#include "alch/systems/broker/receiver.hpp"
#include "alch/systems/broker/sender.hpp"

namespace al {

class Context;

class System: public Receiver, public Sender {
  public:
	System(Context& context);

	System(Context& context, Broker& broker);

	virtual ~System() = default;
	virtual void exportScriptFunctions(Context& context){};

	Context& context() { return _context; }

  protected:
	Context& _context;
};

} // namespace al
