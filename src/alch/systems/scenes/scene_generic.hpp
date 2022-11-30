#pragma once

#include "alch/systems/scenes/scene.hpp"

namespace al {

class Context;
class Entity;

class SceneGeneric: public Scene {
  public:
	template<class FnInit, class FnFrame>
	explicit SceneGeneric(Context& context, const FnInit& fnI, const FnFrame& fnF): Scene(context), fnF(fnF) {
		fnI();
	}

	void onFrame() override {
		if (fnF) {
			fnF();
		}
	};

  private:
	std::function<void()> fnF;
};

} // namespace al
