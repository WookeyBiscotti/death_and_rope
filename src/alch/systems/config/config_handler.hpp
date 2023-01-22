#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/string.hpp"

namespace al {
class ConfigSystem;

class ConfigHandler {
  public:
	ConfigHandler(ConfigSystem& s): _system(s) {}
	ConfigHandler(const ConfigHandler&) = delete;

	~ConfigHandler();

	void subscribe(const String& event, std::function<void(const String&)>);

	void unsubscribe(const String& event);
	void unsubscribe();

  private:
	ConfigSystem& _system;
};

} // namespace al
