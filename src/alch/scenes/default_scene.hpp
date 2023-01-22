#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/render/camera.hpp"
#include "alch/systems/render/render.hpp"
#include "alch/systems/render/sprite_component.hpp"
#include "alch/systems/scenes/scene.hpp"
#include "alch/systems/transform/transform.hpp"
#include "alch/systems/window/inputs.hpp"
#include "alch/systems/window/window.hpp"
//
#include <memory>

namespace al {

class DefaultScene: public Scene {
  public:
	explicit DefaultScene(Context& context): Scene(context){};

	// void active(bool active) override {
	// 	if (active) {
	// 		_root = std::make_unique<Entity>(context());
	// 	} else {
	// 		_root.reset();
	// 	}
	// };

	Entity* root() override { return _root.get(); };

  private:
	std::unique_ptr<Entity> _root;
};

}
