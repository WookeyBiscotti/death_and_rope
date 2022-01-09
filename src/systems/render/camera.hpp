#pragma once

//
#include <common/archive.hpp>
#include <common/rect.hpp>
#include <common/vector2.hpp>
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

	void serialize(IArchive& ar) {
		std::decay_t<decltype(_view.getViewport())> viewport;
		::serialize(ar, viewport);
		_view.setViewport(viewport);

		std::decay_t<decltype(_view.getCenter())> center;
		::serialize(ar, center);
		_view.setCenter(center);

		std::decay_t<decltype(_view.getRotation())> rotation;
		ar(rotation);
		_view.setRotation(rotation);

		std::decay_t<decltype(_view.getSize())> size;
		::serialize(ar, size);
		_view.setSize(size);
	}
	void serialize(OArchive& ar) {
		::serialize(ar, _view.getViewport());
		::serialize(ar, _view.getCenter());
		ar(_view.getRotation());
		::serialize(ar, _view.getSize());
	}

  private:
	sf::View _view;
};