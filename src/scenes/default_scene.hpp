#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/render/camera.hpp>
#include <systems/render/render.hpp>
#include <systems/render/sprite_component.hpp>
#include <systems/scenes/scene.hpp>
#include <systems/transform/transform.hpp>
#include <systems/window/inputs.hpp>
#include <systems/window/window.hpp>
//
#include <memory>

class DefaultScene: public Scene {
  public:
	explicit DefaultScene(Context& context): Scene(context){};

	void active(bool active) override {
		if (active) {
			_root = std::make_unique<Entity>(context());
		} else {
			_root.reset();
		}
	};

	Entity* root() override { return _root.get(); };

  private:
	std::unique_ptr<Entity> _root;
};