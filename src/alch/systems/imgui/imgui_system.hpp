#pragma once

#include "alch/engine/system.hpp"
#include "alch/systems/broker/receiver.hpp"
//
#include <SFML/System/Clock.hpp>

namespace al {

class Context;

class ImGuiSystem: public Receiver, public System {
  public:
	explicit ImGuiSystem(Context& context);

	bool wantCaptureKeyboard() const;
	bool wantCaptureMouse() const;

  private:
	Context& _context;

	sf::Clock _deltaClock;
};

}
