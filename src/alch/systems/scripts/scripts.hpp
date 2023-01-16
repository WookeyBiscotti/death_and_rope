#pragma once

#include "alch/engine/system.hpp"
//
#include <luajit-2.1/lua.hpp>
#include <sol/sol.hpp>

//
#include <memory>
#include <string>

namespace chaiscript {
class ChaiScript;
}

namespace al {

class Scripts: public System {
  public:
	Scripts(Context& context);
	~Scripts();
	void eval(const std::string& command);
	void eval2(const std::string& command);

	chaiscript::ChaiScript& internal() { return *_state; }
	sol::state& internal2() { return _lua; }

	template<class Str, class Fn>
	void add_func(Str&& str, Fn&& fn) {
		_lua.set_function(std::move(str), std::move(fn));
	}

  private:
	std::unique_ptr<chaiscript::ChaiScript> _state;
	sol::state _lua;
};

} // namespace al
