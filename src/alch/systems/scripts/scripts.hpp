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

	chaiscript::ChaiScript& internal() { return *_state; }

  private:
	std::unique_ptr<chaiscript::ChaiScript> _state;
	sol::state lua;
};

}
