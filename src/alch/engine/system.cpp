#include "system.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"

using namespace al;

System::System(Context& context): Transmitter(context.systemRef<Broker>()), _context(context) {
}

System::System(Context& context, Broker& broker): Transmitter(broker), _context(context) {
}
