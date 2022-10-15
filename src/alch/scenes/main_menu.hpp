#pragma once

#include "alch/systems/scenes/scene.hpp"

namespace al {

class MainMenu: public Scene {
  public:
	MainMenu(Context& context): Scene(context) {}

	void onFrame() override;

	bool active() const override;
	void active(bool active) override;

  private:
	bool _active = false;
};

}
