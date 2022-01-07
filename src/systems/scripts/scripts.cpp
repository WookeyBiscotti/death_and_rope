#include "scripts.hpp"

#include <systems/logging/logger.hpp>
//
#include <sol/sol.hpp>
//

int errorHandler(lua_State* L, sol::optional<const std::exception&> e, std::string_view s) {
	LERR_IF(e.has_value(), "Exception: {}", e->what());
	LERR("Error: {}", s);

	lua_pushlstring(L, s.data(), s.size());
	return 1;
}

Scripts::Scripts() {
	_state = std::make_unique<sol::state>();
	_state->open_libraries(sol::lib::base);
	_state->set_exception_handler(&errorHandler);
}

Scripts::~Scripts() {
}

void Scripts::eval(const std::string& command) {
	_state->do_string(command);
}