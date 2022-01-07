#pragma once

#include <engine/system.hpp>
#include <systems/broker/receiver.hpp>
//
#include <SFML/System/Clock.hpp>

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