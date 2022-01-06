#pragma once

#include "drawable.hpp"

#include <common/vector2.hpp>
#include <systems/assets/sprite.hpp>
//
#include <memory>

class SpriteComponent: public Drawable {
  public:
	explicit SpriteComponent(Entity& entity);
	// SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex);
	// SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex, const Recti& rect);

	void setSprite(const std::shared_ptr<Texture>& texture) {
		_texture = texture;
		_sprite.setTexture(texture->sf());
	}
	void setRect(const Recti& rect) { _sprite.setTextureRect(rect); }

	void draw(RenderTarget& target, const RenderStates& state) override;

  private:
	std::shared_ptr<Texture> _texture;
	sf::Sprite _sprite;
};