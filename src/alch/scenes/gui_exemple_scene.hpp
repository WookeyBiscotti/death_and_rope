#pragma once

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/broker/receiver.hpp"
#include "alch/systems/group/group.hpp"
#include "alch/systems/names/name.hpp"
#include "alch/systems/physics/body.hpp"
#include "alch/systems/physics/collider.hpp"
#include "alch/systems/render/camera.hpp"
#include "alch/systems/render/render.hpp"
#include "alch/systems/render/sprite_component.hpp"
#include "alch/systems/scenes/scene.hpp"
#include "alch/systems/transform/transform.hpp"
#include "alch/systems/window/inputs.hpp"
#include "alch/systems/window/window.hpp"
#include "alch/systems/ui/ui_system.hpp"
#include "alch/systems/ui/ui_panel.hpp"
#include "alch/systems/ui/ui_button.hpp"
//
#include <SFML/Graphics/View.hpp>
//
#include <memory>

class GuiExempleScene: public Scene {
  public:
	explicit GuiExempleScene(Context& context): Scene(context){};

	void active(bool active) override {
		if (!active) {
			auto root = context().systemRef<UISystem>().root();
            return;
		}



	};
};
