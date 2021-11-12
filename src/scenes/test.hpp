#pragma once

#include "SFML/Window/Keyboard.hpp"
#include "common/vector2.hpp"
#include "engine/events.hpp"

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/broker/receiver_component.hpp>
#include <systems/position/position.hpp>
#include <systems/render/camera.hpp>
#include <systems/render/render.hpp>
#include <systems/render/sprite_component.hpp>
#include <systems/scenes/scene.hpp>
#include <systems/window/window.hpp>
#include <world.hpp>
//
#include <SFML/Graphics/View.hpp>
//
#include <memory>

class TestScene: public Scene {
  public:
	explicit TestScene(Context& context): Scene(context){};

	void active(bool active) override {
		_head = std::make_unique<Entity>(context());
		_head->add<Position>(Vector2f{300, 300});

		auto c = std::make_unique<SpriteComponent>(*_head);
		c->setSprite(context().systemRef<AssetCache>().sprite("head"));
		_head->add(std::move(c));

		_camera = std::make_unique<Entity>(context());
		_camera->add<Camera>().add<ReceiverComponent>().add<Position>();
		_camera->ref<Camera>().size((Vector2f)context().systemRef<Window>().window().getSize());
		_camera->ref<ReceiverComponent>().subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart& event) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				_camera->ref<Camera>().zoom(1.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
				_camera->ref<Camera>().zoom(0.9);
			}
			auto& pos = _camera->ref<Position>();
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
		context().systemRef<Render>().camera(&_camera->ref<Camera>());
	};

  private:
	std::unique_ptr<Entity> _head;
	std::unique_ptr<Entity> _camera;
};