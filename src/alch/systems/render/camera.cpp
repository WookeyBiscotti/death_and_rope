#include "camera.hpp"

#include "alch/engine/context.hpp"
#include "alch/systems/render/camera.hpp"
#include "alch/systems/render/render.hpp"

using namespace al;

Camera::~Camera() {
	auto& r = entity().context().systemRef<Render>();
	if (r.camera() == this) {
		r.camera(nullptr);
	}
}

void Camera::makeCurrent() {
	auto& r = entity().context().systemRef<Render>();
	r.camera(this);
}

void Camera::save(OArchive& archive) const {
	archive(_view.getViewport(), _view.getCenter(), _view.getRotation(), _view.getSize());
};
void Camera::load(IArchive& archive) {
	std::decay_t<decltype(_view.getViewport())> viewport;
	std::decay_t<decltype(_view.getCenter())> center;
	std::decay_t<decltype(_view.getRotation())> rotation;
	std::decay_t<decltype(_view.getSize())> size;

	archive(viewport, center, rotation, size);

	_view.setViewport(viewport);
	_view.setCenter(center);
	_view.setRotation(rotation);
	_view.setSize(size);
};
