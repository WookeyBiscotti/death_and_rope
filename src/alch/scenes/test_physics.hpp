#pragma once

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/assets/asset_cache.hpp"

#include "alch/systems/group/group.hpp"
#include "alch/systems/names/name.hpp"
#include "alch/systems/physics/body.hpp"
#include "alch/systems/physics/collider.hpp"
#include "alch/systems/render/camera.hpp"
#include "alch/systems/render/render.hpp"
#include "alch/systems/render/sprite_component.hpp"
#include "alch/systems/scenes/scene.hpp"
#include "alch/systems/transform/transform.hpp"
#include "alch/systems/window/inputs.hpp"
#include "alch/systems/window/window.hpp"
//
#include <SFML/Graphics/View.hpp>
//
#include <memory>

namespace al {

class TestPhysicsScene: public Scene {
  public:
	explicit TestPhysicsScene(Context& context): Scene(context){};

	// void active(bool active) override {
	// 	if (!active) {
	// 		_root.reset();
	// 		return;
	// 	}

	// 	_root = std::make_unique<Entity>(context());
	// 	_root->addChain<Group>().addChain<Name>("root");

	// 	auto& objs = _root->ref<Group>().create()->addChain<Group>(Group::SyncMove);
	// 	auto tex = context().systemRef<AssetCache>().texture("head.png");
	// 	for (int i = 0; i != 100; ++i) {
	// 		auto head = objs.ref<Group>().create();
	// 		head->addChain<Sprite>(tex)
	// 		    .addChain<Body>(Body::DYNAMIC)
	// 		    .addChain<Collider>(Collider::Box, Vector2f(30, 30))
	// 		    .ref<Body>()
	// 		    .position(Vector2f(rand() % 450, rand() % 450));
	// 	}

	// 	objs.ref<Group>()
	// 	    .create()
	// 	    ->addChain<Body>(Body::STATIC)
	// 	    .addChain<Collider>(Collider::Box, Vector2f(500, 10))
	// 	    .ref<Body>()
	// 	    .position(Vector2f{0, 510});
	// 	objs.ref<Group>()
	// 	    .create()
	// 	    ->addChain<Body>(Body::STATIC)
	// 	    .addChain<Collider>(Collider::Box, Vector2f(10, 500))
	// 	    .ref<Body>()
	// 	    .position(Vector2f{0, 0});
	// 	objs.ref<Group>()
	// 	    .create()
	// 	    ->addChain<Body>(Body::STATIC)
	// 	    .addChain<Collider>(Collider::Box, Vector2f(10, 500))
	// 	    .ref<Body>()
	// 	    .position(Vector2f{510, 0});

	// 	auto& camera = _root->ref<Group>().create()->addChain<Name>("camera");

	// 	camera.add<Camera>();
	// 	camera.ref<Camera>().size((Vector2f)context().systemRef<Window>().window().getSize());
	// 	subscribe<EngineOnFrameStart>([&camera, this](const EngineOnFrameStart& event) {
	// 		if (Keyboard::isKeyPressed(Keyboard::Q)) {
	// 			camera.ref<Camera>().zoom(1.1);
	// 		}
	// 		if (Keyboard::isKeyPressed(Keyboard::E)) {
	// 			camera.ref<Camera>().zoom(0.9);
	// 		}
	// 		auto& pos = camera.ref<Transform>();
	// 		if (Keyboard::isKeyPressed(Keyboard::A)) {
	// 			pos.p({pos.p().x - 2, pos.p().y});
	// 		}
	// 		if (Keyboard::isKeyPressed(Keyboard::D)) {
	// 			pos.p({pos.p().x + 2, pos.p().y});
	// 		}
	// 		if (Keyboard::isKeyPressed(Keyboard::W)) {
	// 			pos.p({pos.p().x, pos.p().y - 2});
	// 		}
	// 		if (Keyboard::isKeyPressed(Keyboard::S)) {
	// 			pos.p({pos.p().x, pos.p().y + 2});
	// 		}
	// 	});
	// 	context().systemRef<Render>().camera(&camera.ref<Camera>());
	// };

	Entity* root() override { return _root.get(); }

  private:
	std::unique_ptr<Entity> _root;
};

} // namespace al
