#pragma once

#include <memory>
#include <string>

namespace sol {
class state;
}

class Scripts {
  public:
	Scripts();
	~Scripts();
	void eval(const std::string& command);

  private:
	std::unique_ptr<sol::state> _state;
};