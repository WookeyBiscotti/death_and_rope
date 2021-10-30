#pragma once

#include <scene.hpp>
#include <world.hpp>

#include <SFML/Graphics/View.hpp>

#include <context.hpp>
#include <entity.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/position/position.hpp>
#include <systems/renderer/renderer.hpp>
#include <systems/renderer/sprite_component.hpp>
class TestScene : public Scene
{
public:
    TestScene(Context& context)
        : Scene(context){};

    void onFrame() override {}

    void active(bool active) override
    {
        auto c = std::make_shared<SpriteComponent>(_entity);
        c->setSprite(context().systemRef<AssetCache>().sprite("test"));
        _entity.add(std::make_shared<Position>(_entity));
        _entity.get<Position>()->set({ 300, 300 });
        _entity.add(c);
        context().renderer.add(c);
    };

private:
    Entity _entity;
};