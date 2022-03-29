#pragma once
#include "alch/systems/scenes/scene.hpp"

class DevMenu: public Scene {
  public:
	explicit DevMenu(Context& context): Scene(context) {}

	void onFrame() override;

	bool active() const override;
	void active(bool active) override;

  private:
	bool _active = false;
};
