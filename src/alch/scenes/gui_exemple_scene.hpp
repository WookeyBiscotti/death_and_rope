#pragma once

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/group/group.hpp"
#include "alch/systems/names/name.hpp"
#include "alch/systems/physics/body.hpp"
#include "alch/systems/physics/collider.hpp"
#include "alch/systems/render/camera.hpp"
#include "alch/systems/render/render.hpp"
#include "alch/systems/render/sprite_component.hpp"
#include "alch/systems/scenes/scene.hpp"
#include "alch/systems/transform/transform.hpp"
#include "alch/systems/ui/ui_field.hpp"
#include "alch/systems/ui/ui_scroll.hpp"
#include "alch/systems/ui/ui_slider.hpp"
#include "alch/systems/ui/ui_system.hpp"
#include "alch/systems/ui/widgets/ui_button.hpp"
#include "alch/systems/ui/widgets/ui_panel.hpp"
#include "alch/systems/ui/widgets/ui_text.hpp"
#include "alch/systems/window/inputs.hpp"
#include "alch/systems/window/window.hpp"
//
#include <SFML/Graphics/View.hpp>
//

namespace al {

class GuiExempleScene: public Scene {
  public:
	explicit GuiExempleScene(Context& context): Scene(context){};

	// void active(bool active) override {
	// 	if (active) {
	// 		auto root = context().systemRef<UISystem>().root();
	// 		root->layout(UIElement::HORIZONTAL);

	// 		// auto v1 = root->create<UIElement>();
	// 		// auto v2 = root->create<UIScroll>();
	// 		// auto v3 = root->create<UIElement>();

	// 		// v1->layout(UIElement::VERICAL);
	// 		// v2->layout(UIElement::VERICAL);
	// 		// v3->layout(UIElement::VERICAL);

	// 		// auto e1 = v1->create<UIElement>();
	// 		// auto e2 = v1->create<UIPanel>();
	// 		// auto e3 = v1->create<UIText>("qwe", nullptr);

	// 		// auto e4 = v2->create<UIElement>();
	// 		// e4->size({200, 200});
	// 		// auto e5 = v2->create<UIButton>("qwe", nullptr);
	// 		// e5->size({200, 200});
	// 		// auto e6 = v2->create<UIText>("qwe", nullptr);
	// 		// e6->size({200, 200});
	// 		// v2->create<UIButton>("qwe", nullptr)->size({200, 200});
	// 		// v2->create<UIButton>("qwe", nullptr)->size({200, 200});
	// 		// v2->create<UIButton>("qwe", nullptr)->size({200, 200});
	// 		// v2->create<UIButton>("qwe", nullptr)->size({200, 200});
	// 		// v2->create<UIButton>("qwe", nullptr)->size({200, 200});

	// 		// auto e7 = v3->create<UISlider>(5, 0, 10);
	// 		// e7->vertical(true);
	// 		// auto e8 = v3->create<UISlider>(5, 0, 10);
	// 		// auto e9 = v3->create<UIButton>("qwe", nullptr);

	// 		// e9->layout(UIElement::VERICAL);
	// 		// auto b = e9->create<UIButton>("qwe", nullptr);
	// 		// b->size({100, 100});
	// 		// subscribe<UIButtonOnRelease>(b, [this, e9](const UIButtonOnRelease&) { e9->move({40, 40}); });
	// 	} else {
	// 		root()->clear();
	// 	}
	// };
};

} // namespace al
