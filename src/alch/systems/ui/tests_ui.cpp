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
			r->layout(al::UIElement::FREE);
			auto c = r->create<al::UIElement>();
			c->size({100, 100});
			AL_TEST_TRUE(c->size() != r->size());
			AL_TEST_TRUE(c->size() == Vector2<al::UIUnit>(100, 100));
			r->removeAll();
		}
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

ALCH_TEST("UI check layouts for resize") {
	ALCH_IN_FRAME([&](al::Context& ctx) {
		struct TestData {
			struct TestSubData {
				al::UIUnit min;
				al::UIUnit max;
				al::UIUnit result;
			};
			al::UIUnit parentSize;
			std::vector<TestSubData> data;
		};

		auto INF = std::numeric_limits<al::UIUnit>::max();
		std::vector<TestData> data = {{10, {{0, INF, 2}, {0, 1, 1}, {0, INF, 2}, {0, INF, 2}, {0, INF, 2}, {0, 1, 1}}}};

		auto uis = ctx.systemRef<al::UISystem>();

		auto r = uis.root();
		r->layout(al::UIElement::HORIZONTAL);
		for (const auto& d : data) {
			r->size({d.parentSize, 1});
			for (const auto& sd : d.data) {
				auto c = r->create<al::UIElement>();
				c->minSize({sd.min, 1});
				c->maxSize({sd.max, 1});
			}
			size_t idx = 0;
			for (const auto& sd : d.data) {
				AL_TEST_TRUE(r->childs()[idx++]->size().x == sd.result);
			}

			r->removeAll();
		}
	});
}

ALCH_TEST("UI check layouts for gravity") {
	ALCH_IN_FRAME([&](al::Context& ctx) {
		struct TestData {
			struct TestSubData {
				Vector2<al::UIUnit> min;
				Vector2<al::UIUnit> max;
				Vector2<al::UIUnit> resultPos;
			};
			al::UIUnit parentSize;
			std::vector<TestSubData> data;
		};

		constexpr auto H = 3;
		auto INF = std::numeric_limits<al::UIUnit>::max();

		std::vector<TestData> data = {
		    {11, {{{0, 0}, {1, 1}, {1, 1}}, {{0, 0}, {3, INF}, {4, 0}}, {{0, 0}, {1, 1}, {9, 1}}}}};

		auto uis = ctx.systemRef<al::UISystem>();

		auto r = uis.root();
		r->layout(al::UIElement::HORIZONTAL);
		for (const auto& d : data) {
			r->size({d.parentSize, H});
			for (const auto& sd : d.data) {
				auto c = r->create<al::UIElement>();
				c->minSize(sd.min);
				c->maxSize(sd.max);
			}
			size_t idx = 0;
			for (const auto& sd : d.data) {
				AL_TEST_TRUE(r->childs()[idx++]->position() == sd.resultPos);
			}

			r->removeAll();
		}
	});
}
