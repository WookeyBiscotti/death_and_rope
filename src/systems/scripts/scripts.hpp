#pragma once

#include <engine/system.hpp>
//
#include <memory>
#include <string>

namespace sol {
class state;
}

class Scripts: public System {
  public:
	Scripts();
	~Scripts();
	void eval(const std::string& command);

  private:
	std::unique_ptr<sol::state> _state;
};