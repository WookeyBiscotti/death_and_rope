#pragma once

//
#include <common/archive.hpp>
#include <common/rect.hpp>
#include "alch/common/vector2.hpp"
#include <engine/component.hpp>
#include <engine/entity.hpp>
#include <systems/transform/transform.hpp>
//
#include <SFML/Graphics/View.hpp>

class Camera: public Component {
  public:
	explicit Camera(Entity& entity): Component(entity) {}
	~Camera();

	void update() { _view.setCenter(entity().ref<Transform>().p()); }

	void zoom(float factor) { _view.zoom(factor); }

	Vector2f size() const { return _view.getSize(); }
	void size(const Vector2f& val) { return _view.setSize(val); }

	const sf::View& sf() { return _view; }

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

  private:
	sf::View _view;
};
