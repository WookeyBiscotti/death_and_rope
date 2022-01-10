#pragma once

#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/render/camera.hpp>
#include <systems/render/drawable.hpp>
#include <systems/render/sprite_component.hpp>
#include <systems/scenes/scene.hpp>
#include <systems/transform/transform.hpp>
#include <systems/window/inputs.hpp>
#include <systems/window/window.hpp>
//
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
//
#include <memory>

class TestScene: public Scene {
  public:
	explicit TestScene(Context& context): Scene(context){};

	void active(bool active) override {
		if (!active) {
			_head.reset();
			_camera.reset();
			return;
		}
		_head = std::make_unique<Entity>(context());
		_head->add<SpriteComponent>(context().systemRef<AssetCache>().texture("head.png"))
		    .add<Camera>()
		    .ref<Transform>()
		    .p(Vector2f{300, 300});

		_camera = std::make_unique<Entity>(context());
		_camera->add<Camera>();
		_camera->ref<Camera>().size((Vector2f)context().systemRef<Window>().window().getSize());
		_camera->subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart& event) {
			if (Keyboard::isKeyPressed(Keyboard::Q)) {
				_camera->ref<Camera>().zoom(1.1);
			}
			if (Keyboard::isKeyPressed(Keyboard::E)) {
				_camera->ref<Camera>().zoom(0.9);
			}
			auto& pos = _camera->ref<Transform>();
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				pos.p({pos.p().x - 2, pos.p().y});
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				pos.p({pos.p().x + 2, pos.p().y});
			}
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				pos.p({pos.p().x, pos.p().y - 2});
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				pos.p({pos.p().x, pos.p().y + 2});
			}
		});
		context().systemRef<Render>().camera(&_camera->ref<Camera>());
	};

  private:
	std::unique_ptr<Entity> _head;
	std::unique_ptr<Entity> _camera;
};