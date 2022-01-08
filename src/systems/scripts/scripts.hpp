#pragma once

#include <engine/system.hpp>
//
#include <memory>
#include <string>

namespace chaiscript {
class ChaiScript;
}

class Scripts: public System {
  public:
	Scripts();
	~Scripts();
	void eval(const std::string& command);

	chaiscript::ChaiScript& internal() { return *_state; }

  private:
	std::unique_ptr<chaiscript::ChaiScript> _state;
};