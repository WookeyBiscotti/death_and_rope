#pragma once

#include "SFML/Window/Keyboard.hpp"
#include "player.hpp"

#include <engine/component.hpp>
#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/broker/receiver.hpp>
#include <systems/position/position.hpp>

class Player: public Component, public Receiver {
  public:
	Player(Entity& entity): Receiver(entity.context().systemRef<Broker>()), Component(entity) {
		auto& pos = *entity.get<Position>();
		subscribe<EngineOnFrameStart>([&pos = pos](const EngineOnFrameStart&) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				pos.set({pos.get().x - 2, pos.get().y});
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				pos.set({pos.get().x + 2, pos.get().y});
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				pos.set({pos.get().x, pos.get().y - 2});
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				pos.set({pos.get().x, pos.get().y + 2});
			}
		});
	}
};