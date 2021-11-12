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