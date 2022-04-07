#pragma once

#include "alch/common/macros.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/systems/assets/texture.hpp"
#include "alch/systems/broker/receiver.hpp"
#include "drawable.hpp"
//
#include <SFML/Graphics/Sprite.hpp>
//
#include <memory>

class Sprite: public Drawable, public Receiver{
  public:
	explicit Sprite(Entity& entity);
	Sprite(Entity& entity, const std::shared_ptr<Texture>& tex);
	Sprite(Entity& entity, const std::shared_ptr<Texture>& tex, const Recti& rect);

	void texture(const std::shared_ptr<Texture>& texture) {
		_texture = texture;
		_sprite.setTexture(texture->sf());
	}
	const std::shared_ptr<Texture>& texture() const { return _texture; }

	void rect(const Recti& rect) { _sprite.setTextureRect(rect); }
	Recti rect() { return _sprite.getTextureRect(); }

	void draw(RenderTarget& target, const RenderStates& state) override;

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

	ALCH_COMPONENT_NAME(Sprite);

  private:
	std::shared_ptr<Texture> _texture;
	sf::Sprite _sprite;
};
