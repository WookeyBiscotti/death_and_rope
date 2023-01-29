#include "config.hpp"
//
#include "alch/common/vector2.hpp"
//
#include "alch/common/containers/recursive_tree.hpp"
#include "alch/common/file.hpp"
#include "alch/common/json.hpp"
#include "alch/engine/context.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/config/config_defs.hpp"
#include "alch/systems/config/config_tree.hpp"
#include "alch/systems/config/config_value.hpp"
#include "alch/systems/logging/logger.hpp"
#include "events.hpp"
//
#include "alch/common/archive.hpp"
//
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/specialize.hpp>
#include <filesystem>
#include <fstream>
#include <future>
#include <unordered_map>
//

#if defined(linux) || defined(__APPLE__)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

using namespace al;

std::string getHomeDir() {
	const char* homedir;

	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}

	return homedir;
}
#else
static_assert(false, "define for other platforms");
#endif

using namespace std::literals::string_literals;
namespace fs = std::filesystem;

static const auto CONFIG_PATH = "config_path"s;
static const auto ROOT = "root"s;

auto parseArgs(const char** argv, int argc) {
	std::unordered_map<std::string, std::string> res;
	for (int i = 0; i != argc;) {
		auto a = argv[i++];
		if (a == CONFIG_PATH || a == ROOT) {
			if (i == argc) {
				LERR("Wrong program arg: {}", a);
				break;
			}
			res[a] = argv[i++];
		}
	}

	return res;
}

al::ConfigSystem::ConfigTree ConfigSystem::createDefaultConfig() {
	using namespace al::config;
	ConfigTree config;

	auto setter = [&config]<class T, const char name[]>(SystemConfigValue::Access access) {
		config[name] = SystemConfigValue{access, defaultValue<T, name>};
	};

	setter.template operator()<Vector2f, WINDOW_SIZE>(SystemConfigValue::USER);
	setter.template operator()<bool, WINDOW_FULLSCREEN>(SystemConfigValue::USER);
	setter.template operator()<String, APPLICATION_NAME>(SystemConfigValue::USER);

	return config;
}

ConfigSystem::ConfigSystem(Context& context, const ConfigTree& startConfig, const char** argv, int argc):
    System(context) {
	_pathToApp = fs::path(argv[0]);

	auto args = parseArgs(argv, argc);

	if (args.contains(CONFIG_PATH)) {
		_configPath = args[CONFIG_PATH];
	}
	if (_configPath.empty()) {
		_configPath = getHomeDir();
		_configPath /= "";
		_configPath /= "config.json";

		if (!fs::exists(_configPath.parent_path()) && !fs::create_directories(_configPath.parent_path())) {
			LERR("Can't create config dir: {}", _configPath.parent_path().string());
		}
	}

	LINFO("Config path: {}", _configPath.string());

	// if (!_staticConfig.fromString(readFromFile(_configPath))) {
	// 	LERR("Can't load config: {}", _configPath.string());
	// 	asyncSave();
	// } else {
	// 	LINFO("Config: {}", _staticConfig.toString());
	// }

	// if (_staticConfig.root.empty()) {
	// 	// _staticConfig.root = appPath.parent_path().string();
	// }

	// LINFO("App root: {}", _staticConfig.root.string());

	if (args.contains(ROOT)) {
		fs::path newRootPath(args[ROOT]);
		if (fs::is_directory(newRootPath)) {
			// _staticConfig.root = appPath.parent_path().string();
		} else {
			LERR("Wrong path for root: {}", newRootPath.string());
		}
	}
}

void ConfigSystem::asyncConfigSave() {
	_asyncOps.remove_if([](auto& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; });

	_asyncOps.push_back(std::async(std::launch::async, [this]() {
		using namespace al::config;
		const auto configPath = value<String>(USER_DATA_PATH);
		if (configPath) {
			std::ofstream stream(configPath->c_str());
			OJSONArchive ar(stream);
			ar(_userConfig);
		} else {
			LERR("Cant save config: no config path in variables");
		}
	}));
}

// void ConfigSystem::staticConfig(const StaticConfig& newConfig) {
// 	if (newConfig != _staticConfig) {
// 		send(ConfigOnChange{});
// 	}

// 	_staticConfig = newConfig;
// 	asyncSave();
// }

const ConfigValue* ConfigSystem::valueVariant(const String& name) const {
	auto uFound = _userConfig.find(name);
	if (uFound) {
		return uFound;
	}
	auto sFound = _systemConfig.find(name);
	if (sFound) {
		return &sFound->value;
	}

	return nullptr;
}

ConfigValue* ConfigSystem::valueVariant(const String& name) {
	return const_cast<ConfigValue*>(static_cast<const ConfigSystem*>(this)->valueVariant(name));
}

void ConfigSystem::subscribe(ConfigHandler& handler, const String& event, std::function<void(const String&)> fn) {
	_eventToHandler[event].emplace(&handler, std::move(fn));
	_handlerToEvent[&handler].insert(event);
}

void ConfigSystem::unsubscribe(ConfigHandler& handler, const String& event) {
	auto foundH = _handlerToEvent.find(&handler);
	if (foundH != _handlerToEvent.end()) {
		foundH->second.erase(event);
		auto foundE = _eventToHandler.find(event);
		if (foundE != _eventToHandler.end()) {
			foundE->second.erase(&handler);
		}
	}
}

void ConfigSystem::unsubscribe(ConfigHandler& handler) {
	auto foundH = _handlerToEvent.find(&handler);
	if (foundH != _handlerToEvent.end()) {
		for (const auto& e : foundH->second) {
			_eventToHandler[e].erase(&handler);
		}
	}
	_handlerToEvent.erase(&handler);
}
