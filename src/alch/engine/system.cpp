#include "system.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"

using namespace al;

System::System(Context& context):
    Receiver(context.systemRef<Broker>()), Sender(context.systemRef<Broker>()), _context(context) {
}

System::System(Context& context, Broker& broker): Receiver(broker), Sender(broker), _context(context) {
}
