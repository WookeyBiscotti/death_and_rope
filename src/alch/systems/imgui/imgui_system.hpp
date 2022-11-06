#pragma once

#include "alch/engine/system.hpp"

//
#include <SFML/System/Clock.hpp>

namespace al {

class Context;

class ImGuiSystem: public System {
  public:
	explicit ImGuiSystem(Context& context);

	bool wantCaptureKeyboard() const;
	bool wantCaptureMouse() const;

  private:
	sf::Clock _deltaClock;
};

} // namespace al
