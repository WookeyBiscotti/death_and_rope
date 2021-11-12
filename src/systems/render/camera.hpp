#pragma once

#include <common/vector2.hpp>
//
#include <SFML/Graphics/View.hpp>
//
#include <engine/component.hpp>
#include <engine/entity.hpp>
#include <systems/position/position.hpp>

class Camera: public Component {
  public:
	explicit Camera(Entity& entity): Component(entity) {}
	~Camera();

	void update() { _view.setCenter(entity().ref<Position>().get()); }

	void zoom(float factor) { _view.zoom(factor); }

	Vector2f size() const { return _view.getSize(); }
	void size(const Vector2f& val) { return _view.setSize(val); }

	const sf::View& sf() { return _view; }

  private:
	sf::View _view;
};