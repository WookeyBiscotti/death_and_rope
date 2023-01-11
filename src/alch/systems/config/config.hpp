#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/types.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/config/config_tree.hpp"

//
#include <future>
#include <list>
//
#include "alch/common/archive.hpp"

namespace al {

struct StaticConfig {
	Path root;

	std::string appName;

	struct Window {
		Vector2u size{800, 600};
		Vector2u position{0, 0};
		bool fullscreen{};
		bool borderless{};
		bool verticalSync{};
		bool operator==(const Window& other) const = default;
	} window;

	bool operator==(const StaticConfig& other) const = default;

	std::string toString() const;
	bool fromString(const std::string& str);

	Path _assetsPath;
	Path _userPath;

	template<class Archive>
	void serialize(Archive& ar) {
		ar(_assetsPath, _userPath);
	}
};

class Context;
class Config: public System {
  public:
	Config(Context& context, const char** argv, int argc);

	const StaticConfig& staticConfig() const { return _staticConfig; }
	void staticConfig(const StaticConfig& newConfig);

	void asyncSave();

  private:
	Path _configPath;

	StaticConfig _staticConfig;

	std::list<std::future<void>> _asyncOps;

	Path _pathToApp;
	Path _assetsPath;
	Path _userPath;

	ConfigTree _systemConfig;
	ConfigTree _userConfig;
};

} // namespace al
