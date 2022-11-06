#pragma once

#include "alch/systems/broker/transmitter.hpp"

namespace al {

class Context;

class System: public Transmitter {
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
