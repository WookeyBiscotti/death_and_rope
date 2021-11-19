#pragma once

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/group/group.hpp>
#include <systems/physics/body.hpp>
#include <systems/physics/collider.hpp>
#include <systems/position/position.hpp>
#include <systems/render/camera.hpp>
#include <systems/render/render.hpp>
#include <systems/render/sprite_component.hpp>
#include <systems/scenes/scene.hpp>
#include <systems/window/inputs.hpp>
#include <systems/window/window.hpp>
//
#include <SFML/Graphics/View.hpp>
//
#include <memory>

class TestPhysicsScene: public Scene {
  public:
	explicit TestPhysicsScene(Context& context): Scene(context) {
		_root = std::make_unique<Entity>(context);
		_root->add<Group>(Group::SYNC_MOVE).add<Position>();
	};

	void active(bool active) override {
		for (int i = 0; i != 10; ++i) {
			auto& head = _root->ref<Group>().create();
			auto c = std::make_unique<SpriteComponent>(head);
			c->setSprite(context().systemRef<AssetCache>().sprite("head"));

			head.add<Position>(Vector2f(rand() % 500, rand() % 500))
			    .add(std::move(c))
			    .add<Collider>(Vector2f(30, 30))
			    .add<Body>();
			head.ref<Body>().velocity(10.0f * Vector2f((100 - rand() % 200) / 100.f, (100 - rand() % 200) / 100.f));
		}
		_root->ref<Group>().create().add<Position>(0, 0).add<Collider>(Vector2f(10, 500)).add<Body>(Body::STATIC);
		_root->ref<Group>().create().add<Position>(0, 0).add<Collider>(Vector2f(500, 10)).add<Body>(Body::STATIC);
		_root->ref<Group>().create().add<Position>(500, 0).add<Collider>(Vector2f(10, 500)).add<Body>(Body::STATIC);
		_root->ref<Group>().create().add<Position>(0, 500).add<Collider>(Vector2f(500, 10)).add<Body>(Body::STATIC);

		_root->subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart& event) {
			auto& pos = _root->ref<Position>();
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				pos.set({pos.get().x - 2, pos.get().y});
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				pos.set({pos.get().x + 2, pos.get().y});
			}
			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				pos.set({pos.get().x, pos.get().y - 2});
			}
			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				pos.set({pos.get().x, pos.get().y + 2});
			}
		});

		_camera = std::make_unique<Entity>(context());
		_camera->add<Camera>().add<Position>();
		_camera->ref<Camera>().size((Vector2f)context().systemRef<Window>().window().getSize());
		_camera->subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart& event) {
			if (Keyboard::isKeyPressed(Keyboard::Q)) {
				_camera->ref<Camera>().zoom(1.1);
			}
			if (Keyboard::isKeyPressed(Keyboard::E)) {
				_camera->ref<Camera>().zoom(0.9);
			}
			auto& pos = _camera->ref<Position>();
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				pos.set({pos.get().x - 2, pos.get().y});
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				pos.set({pos.get().x + 2, pos.get().y});
			}
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				pos.set({pos.get().x, pos.get().y - 2});
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				pos.set({pos.get().x, pos.get().y + 2});
			}
		});
		context().systemRef<Render>().camera(&_camera->ref<Camera>());
	};

  private:
	std::unique_ptr<Entity> _root;
	std::unique_ptr<Entity> _camera;
};