#include <alch/engine/context.hpp>
#include <alch/engine/engine.hpp>
#include <alch/systems/scenes/scene_generic.hpp>
#include <alch/systems/config/config.hpp>
#include <alch/systems/scenes/scene_system.hpp>
#include <alch/systems/ui/ui_element.hpp>
#include <alch/systems/ui/ui_system.hpp>
#include <alch/systems/ui/widgets/ui_button.hpp>
#include <alch/systems/ui/widgets/ui_input.hpp>
#include <alch/systems/ui/widgets/ui_panel.hpp>
#include <alch/systems/ui/widgets/ui_text.hpp>
#include <alch/systems/ui/widgets/ui_window.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

using namespace al;

int main(int argc, const char** argv) {
	auto context = std::make_unique<Context>();

	ConfigSystem::ConfigTree config;
	auto& e = context->createSystem<Engine>(config, argv, argc);

	auto scene = SharedPtr<SceneGeneric>::make(
			    *context,
			    [&c = *context] {
				    auto uis = c.systemRef<al::UISystem>();

				    auto r = uis.root();
				    {
					    r->distanceBetweenChildren(10);
					    r->indentTop(5);
					    r->indentBot(15);
					    r->indentLeft(20);
					    r->indentRight(40);
					    r->layout(al::UIElement::VERICAL);
					    r->create<al::UIPanel>();
					    auto p = r->create<al::UIPanel>();
					    p->maxSize({40, 40});
					    auto t = r->create<al::UIText>();
					    t->content("WEQWEQWEWQE");
					    t->maxSize({UIUnitMax, 30});
					    r->create<al::UIPanel>();
					    r->create<al::UIButton>("Wtf");
					    r->create<al::UIInput>("йцу");
				    }
				    {
					    auto w = uis.popout()->create<UIWindow>("Test window");
					    w->minSize({200, 200});
					    w->position({100, 100});
					    w->layout(UIElement::HORIZONTAL);
						w->distanceBetweenChildren(5);
						w->indentLeft(10);
						w->indentRight(10);
						w->indentTop(10);
						w->indentBot(10);
					    auto p = w->create<UIPanel>();
					    w->create<UIButton>("Test btn");
				    }
			    },
			    [&c = *context]() {});
	e.run(scene);

	return 0;
}
