#pragma once

#include "alch/common/macros.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/vector2.hpp"
#include "alch/systems/assets/texture.hpp"
#include "alch/systems/broker/transmitter.hpp"
#include "drawable.hpp"
//
#include "alch/common/archive.hpp"
//
#include <SFML/Graphics/Sprite.hpp>
//
#include <memory>

namespace al {

class Sprite: public Drawable, public Transmitter {
  public:
	AL_OBJECT_REGISTER();

	explicit Sprite(Entity& entity);
	Sprite(Entity& entity, const SharedPtr<Texture>& tex);
	Sprite(Entity& entity, const SharedPtr<Texture>& tex, const Recti& rect);

	void texture(const SharedPtr<Texture>& texture) {
		_texture = texture;
		_sprite.setTexture(texture->sf());
	}
	const SharedPtr<Texture>& texture() const { return _texture; }

	void rect(const Recti& rect) { _sprite.setTextureRect(rect); }
	Recti rect() { return _sprite.getTextureRect(); }

	void draw(RenderTarget& target, const RenderStates& state) override;

	void save(OArchive& archive) const override;
	void load(IArchive& archive) override;

  private:
	SharedPtr<Texture> _texture;
	sf::Sprite _sprite;
};

} // namespace al
