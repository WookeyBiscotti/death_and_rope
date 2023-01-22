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

class EditorScene: public Scene {
  public:
	explicit EditorScene(Context& context): Scene(context){};

	void onFrame() override;

	Entity* root() override { return _root.get(); };

  private:
	Entity* _selected{};
	// return - if node is deleted
	bool drawNode(Entity& entity, Entity*& selected);

  private:
	std::unique_ptr<Entity> _root;
};

}
