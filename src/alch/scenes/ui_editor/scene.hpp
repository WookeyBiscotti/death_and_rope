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

namespace al {

class UIEditorScene: public Scene {
  public:
	explicit UIEditorScene(Context& context): Scene(context){};

	// void active(bool active) override;

	void onFrame() override;

	Entity* root() override { return nullptr; };

  private:
};

} // namespace al
