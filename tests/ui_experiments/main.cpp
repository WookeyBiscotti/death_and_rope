#include <alch/engine/context.hpp>
#include <alch/engine/engine.hpp>
#include <alch/systems/scenes/scene_generic.hpp>
#include <alch/systems/scenes/scene_system.hpp>
#include <alch/systems/ui/ui_element.hpp>
#include <alch/systems/ui/ui_system.hpp>
#include <alch/systems/ui/widgets/ui_panel.hpp>
#include <alch/systems/ui/widgets/ui_button.hpp>
#include <alch/systems/ui/widgets/ui_text.hpp>
#include <alch/systems/ui/widgets/ui_input.hpp>

using namespace al;

int main(int argc, const char** argv) {
	EngineConfig config;
	config.enableDefaultScenes = false;
	config.windowName = "Tests";
	config.startScene = "Tests";
	config.preBegin = [&](Context& c) {
		c.system<SceneSystem>()->registerScene("Tests", [&c]() {
			return std::make_shared<SceneGeneric>(
			    c,
			    [&c] {
				    auto uis = c.systemRef<al::UISystem>();

				    auto r = uis.root();
				    r->layout(al::UIElement::VERICAL);
				    r->create<al::UIPanel>();
				    auto p = r->create<al::UIPanel>();
				    p->maxSize({40, 40});
				    auto t = r->create<al::UIText>();
					t->content("WEQWEQWEWQE");
					t->maxSize({UIUnitMax, 30});
				    // r->create<al::UIPanel>();
					r->create<al::UIButton>("Wtf");
					r->create<al::UIInput>("йцу");
			    },
			    [&c]() {});
		});
	};

	auto context = std::make_unique<Context>();

	auto& e = context->createSystem<Engine>();
	e.run(argv, argc, config);

	return 0;
}
