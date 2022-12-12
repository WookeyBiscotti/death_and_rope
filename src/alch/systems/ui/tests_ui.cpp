#include "alch/common/test_framework.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/engine.hpp"
#include "alch/systems/scenes/scene_generic.hpp"
#include "alch/systems/scenes/scene_system.hpp"
#include "alch/systems/ui/ui_element.hpp"
#include "alch/systems/ui/ui_system.hpp"

ALCH_TEST("UI check layouts for simple widgets") {
	ALCH_IN_FRAME([&](al::Context& ctx) {
		auto uis = ctx.systemRef<al::UISystem>();
		auto r = uis.root();
		{
			r->layout(al::UIElement::HORIZONTAL);
			auto c = r->create<al::UIElement>();
			AL_TEST_TRUE(c->size() == r->size());
			r->removeAll();
		}
		{
			r->layout(al::UIElement::VERICAL);
			auto c = r->create<al::UIElement>();
			AL_TEST_TRUE(c->size() == r->size());
			r->removeAll();
		}
		{
			r->layout(al::UIElement::FREE);
			auto c = r->create<al::UIElement>();
			AL_TEST_FALSE(c->size() == r->size());
			r->removeAll();
		}

		{
			r->layout(al::UIElement::VERICAL);

			const size_t count = 10;

			for (size_t i = 0; i != count; ++i) {
				r->create<al::UIElement>();
			}

			for (const auto& c : r->childs()) {
				AL_TEST_TRUE(c->size().x == r->size().x);
				AL_TEST_TRUE(c->size().y == r->size().y / count);
			}

			r->removeAll();
		}

		{
			r->layout(al::UIElement::HORIZONTAL);

			const size_t count = 20;

			for (size_t i = 0; i != count; ++i) {
				r->create<al::UIElement>();
			}

			for (const auto& c : r->childs()) {
				AL_TEST_TRUE(c->size().y == r->size().y);
				AL_TEST_TRUE(c->size().x == r->size().x / count);
			}

			r->removeAll();
		}
	});
}
