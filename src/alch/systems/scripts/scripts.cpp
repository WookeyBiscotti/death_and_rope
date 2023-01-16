#include "scripts.hpp"

#include "alch/systems/logging/logger.hpp"
//
#include <chaiscript/chaiscript.hpp>
//

using namespace al;

Scripts::Scripts(Context& context): System(context) {
	using namespace chaiscript;

	_state = std::make_unique<chaiscript::ChaiScript>();

	_lua.open_libraries(sol::lib::base, sol::lib::package);

	auto lib = std::make_shared<Module>();
	bootstrap::standard_library::vector_type<std::vector<std::string>>("StringVector", *lib);
	_state->add(lib);
	_state->add(fun([](const std::vector<std::string>& v) {
		std::string str;
		for (const auto& s : v) {
			str += s;
			if (&s != &v.back()) {
				str += ", ";
			}
		}
		return str;
	}),
	    "to_string");
}

Scripts::~Scripts() = default;

void Scripts::eval(const std::string& command) {
	// TODO: Do smth here
	try {
		auto res = _state->eval(command);
		// LERR("Executing error: {}", res);
	} catch (const std::exception& e) { LERR("Executing error: {}", e.what()); }
}

void Scripts::eval2(const std::string& command) {
	// TODO: Do smth here
	try {
		auto res = _lua.script(command);
	} catch (const std::exception& e) { LERR("Executing error: {}", e.what()); }
}
