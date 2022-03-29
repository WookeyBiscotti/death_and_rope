
#include "arch/engine/component.hpp"
#include "arch/engine/context.hpp"
#include "arch/engine/entity.hpp"
#include "arch/systems/broker/broker.hpp"

#include <catch2/catch_test_macros.hpp>
//
#include <memory>

template<size_t N>
class ComponentNum: public Component {
  public:
	explicit ComponentNum(Entity& e, size_t val): Component(e), val(val){};

	constexpr auto num() { return N; };

	size_t val = 0;
};

TEST_CASE("General", "[Entity]") {
	Broker broker;
	Context context;
	context.addSystem(&broker);
	Entity entity(context);

	auto c2 = std::make_unique<ComponentNum<2>>(entity, 17);

	entity.add<ComponentNum<1>>(13).add(std::move(c2));

	SECTION("Check component") {
		REQUIRE(entity.get<ComponentNum<1>>());
		REQUIRE(entity.get<ComponentNum<2>>());

		REQUIRE(entity.get<ComponentNum<1>>()->val == 13);
		REQUIRE(entity.get<ComponentNum<2>>()->val == 17);
	}

	SECTION("Check component val") {
		REQUIRE(entity.get<ComponentNum<1>>()->val == 13);
		REQUIRE(entity.get<ComponentNum<2>>()->val == 17);

		REQUIRE(entity.ref<ComponentNum<1>>().val == 13);
		REQUIRE(entity.ref<ComponentNum<2>>().val == 17);
	}

	SECTION("Check component val") {
		REQUIRE(entity.remove<ComponentNum<1>>());
		REQUIRE(entity.remove<ComponentNum<2>>());

		REQUIRE_FALSE(entity.get<ComponentNum<1>>());
		REQUIRE_FALSE(entity.get<ComponentNum<2>>());
	}
}
