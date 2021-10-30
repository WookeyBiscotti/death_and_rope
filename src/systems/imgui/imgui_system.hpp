#pragma once

#include <systems/broker/receiver.hpp>
//
#include <SFML/System/Clock.hpp>

class Context;

class ImGuiSystem: public Receiver {
  public:
	ImGuiSystem(Context& context);

	void update();

	void render();

  private:
	Context& _context;

	sf::Clock _deltaClock;
};