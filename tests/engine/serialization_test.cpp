#include "alch/common/type_id.hpp"
#include "alch/systems/transform/transform.hpp"
#include "box2d/b2_user_settings.h"

#include <alch/engine/component.hpp>
#include <alch/engine/context.hpp>
#include <alch/engine/entity.hpp>
#include <alch/systems/broker/broker.hpp>
//
#include <catch2/catch_test_macros.hpp>
//
#include <memory>
#include <sstream>

template<size_t N>
class ComponentNum1: public Component {
  public:
	explicit ComponentNum1(Entity& e, size_t val = 0): Component(e), val(val){};

	constexpr auto num() { return N; };

	size_t val = 0;

	void serialize(OArchive& archive) const { archive(val); }
	void deserialize(IArchive& archive) {
		if (N == 2) {
			auto comm2 = entity().template get<ComponentNum1<2>>();
			REQUIRE(comm2 != nullptr);
		}
		archive(val);
	};

	std::string_view cName() const override {       
		static const std::string name = "ComponentNum1" + std::to_string(N); 
		return name;                                
	};
};

TEST_CASE("General", "[Serialize]") {
	Broker broker;
	Logger logger;

	Context context;
	context.addSystem(&logger);
	context.addSystem(&broker);
	Entity::registerComponent<Transform>("Transform");

	Entity entity(context);

	Entity::registerComponent<ComponentNum1<1>>("Comm1");
	Entity::registerComponent<ComponentNum1<2>>("Comm2", TypeId<ComponentNum1<1>>());

	entity.add<ComponentNum1<1>>(13);
	entity.add<ComponentNum1<2>>(77);

	std::stringstream ss;
	OArchive ia(ss);
	entity.serialize(ia);
	entity.clear();

	IArchive oa(ss);
	entity.deserialize(oa);

	REQUIRE(entity.ref<ComponentNum1<1>>().val == 13);
	REQUIRE(entity.ref<ComponentNum1<2>>().val == 77);
}
