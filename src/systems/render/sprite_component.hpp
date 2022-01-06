#pragma once

#include "drawable.hpp"

#include <common/macros.hpp>
#include <common/rect.hpp>
#include <common/vector2.hpp>
#include <systems/assets/texture.hpp>
//
#include <SFML/Graphics/Sprite.hpp>
//
#include <memory>

class SpriteComponent: public Drawable {
  public:
	explicit SpriteComponent(Entity& entity);
	SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex);
	SpriteComponent(Entity& entity, const std::shared_ptr<Texture>& tex, const Recti& rect);

	void texture(const std::shared_ptr<Texture>& texture) {
		_texture = texture;
		_sprite.setTexture(texture->sf());
	}
	const std::shared_ptr<Texture>& texture() const { return _texture; }

	void rect(const Recti& rect) { _sprite.setTextureRect(rect); }
	Recti rect() { return _sprite.getTextureRect(); }

	void draw(RenderTarget& target, const RenderStates& state) override;

  private:
	std::shared_ptr<Texture> _texture;
	sf::Sprite _sprite;
};