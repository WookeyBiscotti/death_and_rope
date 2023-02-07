#pragma once

//
#include "alch/common/rect.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/transform/transform.hpp"
//
#include "alch/common/archive.hpp"
//
#include <SFML/Graphics/View.hpp>

namespace al {

class Camera: public Component {
  public:
	AL_OBJECT_REGISTER();

	explicit Camera(Entity& entity): Component(entity) {}
	~Camera();

	void update() { _view.setCenter(entity().ref<Transform>().p()); }

	void zoom(float factor) { _view.zoom(factor); }

	Vector2f size() const { return _view.getSize(); }
	void size(const Vector2f& val) { return _view.setSize(val); }

	const sf::View& sf() { return _view; }

	void makeCurrent();

	void save(OArchive& archive) const override;
	void load(IArchive& archive) override;

  private:
	sf::View _view;
};

} // namespace al
