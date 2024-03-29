#pragma once

#include "alch/common/color.hpp"
#include "alch/common/macros.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/systems/assets/texture.hpp"
#include "alch/systems/broker/transmitter.hpp"
#include "drawable.hpp"
//
#include "alch/common/archive.hpp"
//
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
//

namespace al {

class RectShape: public Drawable, public Transmitter {
  public:
	AL_OBJECT_REGISTER();

	explicit RectShape(Entity& entity);
	RectShape(Entity& entity, Vector2f size, sf::Color color);

	void draw(RenderTarget& target, const RenderStates& state) override;

	void save(OArchive& archive) const override;
	void load(IArchive& archive) override;

  private:
	sf::RectangleShape _shape;
};

} // namespace al
