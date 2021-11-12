#pragma once

#include "common/vector2.hpp"

#include <components/player.hpp>
#include <engine/context.hpp>
#include <engine/entity.hpp>
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
	explicit TestScene(Context& context): Scene(context){};

	void active(bool active) override {
		_entity = std::make_unique<Entity>(context());
		_entity->add<Position>(Vector2f{300, 300}).add<Player>();

		auto c = std::make_unique<SpriteComponent>(*_entity);
		c->setSprite(context().systemRef<AssetCache>().sprite("head"));
		_entity->add(std::move(c));
	};

  private:
	std::unique_ptr<Entity> _entity;
};