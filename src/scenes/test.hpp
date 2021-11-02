#pragma once

#include <components/player.hpp>
#include <context.hpp>
#include <entity.hpp>
#include <memory>
#include <systems/assets/asset_cache.hpp>
#include <systems/position/position.hpp>
#include <systems/render/render.hpp>
#include <systems/render/sprite_component.hpp>
#include <systems/scenes/scene.hpp>
#include <world.hpp>
//
#include <SFML/Graphics/View.hpp>

class TestScene: public Scene {
  public:
	TestScene(Context& context): Scene(context){};

	void onFrame() override {}

	void active(bool active) override {
		_entity = std::make_unique<Entity>(context());
		auto c = std::make_shared<SpriteComponent>(*_entity);
		c->setSprite(context().systemRef<AssetCache>().sprite("head"));
		_entity->add(std::make_shared<Position>(*_entity));
		_entity->get<Position>()->set({300, 300});
		_entity->add(c);
		context().systemRef<Render>().add(c);
		auto p = std::make_shared<Player>(*_entity);
		_entity->add(p);
	};

  private:
	std::unique_ptr<Entity> _entity;
};