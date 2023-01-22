#pragma once
#include "alch/systems/scenes/scene.hpp"

namespace al {

class DevMenu: public Scene {
  public:
	explicit DevMenu(Context& context): Scene(context) {}

	void onFrame() override;
};
} // namespace al
