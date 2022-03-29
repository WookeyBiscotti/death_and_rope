#include <arch/engine/component.hpp>
#include <arch/engine/context.hpp>
#include <arch/engine/entity.hpp>
#include <arch/systems/broker/broker.hpp>

#include <catch2/catch_test_macros.hpp>
//
#include <memory>

struct Event1 {
	int i;
};
struct Event2 {
	std::string s;
};

TEST_CASE("General", "[Broker]") {
	Broker broker;

	Receiver r(broker);
	Sender s(broker);

	SECTION("Subs undirect") {
		bool testPass = false;
		r.subscribe<Event1>([&testPass](const auto& e) { testPass = true; });
		s.send(Event1{});
		REQUIRE(testPass);
	}
	SECTION("Unsubscribe undirect") {
		bool testPass = true;
		r.subscribe<Event1>([&testPass](const auto& e) { testPass = false; });
		r.unsubscribe<Event1>();
		s.send(Event1{});
		REQUIRE(testPass);
	}
	SECTION("Wrong unsuscription") {
		r.unsubscribe<Event1>();
		// No crash
	}
	SECTION("Multi suscription") {
		int i = 0;
		r.subscribe<Event1>(&s, (void*)1, [&i](const Event1&) { i = 17; });
		r.subscribe<Event1>(&s, (void*)2, [&i](const Event1&) { i = 13; });
		r.unsubscribe<Event1>(&s, (void*)1);
		s.send(Event1{});
		REQUIRE(i == 13);
	}
	SECTION("Multi suscription") {
		int i = 0;
		r.subscribe<Event1>(&s, (void*)1, [&i](const Event1&) { i += 17; });
		r.subscribe<Event1>(&s, (void*)2, [&i](const Event1&) { i += 13; });
		s.send(Event1{});
		REQUIRE(i == 30);
		r.unsubscribe<Event1>(&s, (void*)1);
		s.send(Event1{});
		REQUIRE(i == 43);
	}
}
