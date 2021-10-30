#pragma once
#include <systems/scenes/scene.hpp>

class DevMenu: public Scene {
  public:
	DevMenu(Context& context): Scene(context) {}

	void onFrame() override;

	bool active() const override;
	void active(bool active) override;

  private:
	bool _active = false;
};