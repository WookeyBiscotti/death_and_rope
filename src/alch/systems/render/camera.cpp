#include "camera.hpp"

#include "systems/render/camera.hpp"
#include "systems/render/render.hpp"

#include <cstddef>
#include <engine/context.hpp>

Camera::~Camera() {
	auto& r = entity().context().systemRef<Render>();
	if (r.camera() == this) {
		r.camera(nullptr);
	}
}

void Camera::deserialize(IArchive& ar) {
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

void Camera::serialize(OArchive& ar) const {
	::serialize(ar, _view.getViewport());
	::serialize(ar, _view.getCenter());
	ar(_view.getRotation());
	::serialize(ar, _view.getSize());
}
