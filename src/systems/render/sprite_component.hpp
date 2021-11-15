#pragma once

#include "drawable.hpp"

#include <common/vector2.hpp>
#include <systems/assets/sprite.hpp>
//
#include <memory>

class SpriteComponent: public Drawable {
  public:
	explicit SpriteComponent(Entity& entity): Drawable(entity) {}

	void setSprite(const std::shared_ptr<Sprite>& sprite);

	void draw(RenderTarget& target, const RenderStates& state) override;

  private:
	std::shared_ptr<Sprite> _sprite;
};