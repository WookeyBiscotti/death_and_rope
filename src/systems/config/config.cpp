#include "config.hpp"

#include <file.hpp>
#include <json.hpp>
#include <systems/logging/logger.hpp>
//
#include <filesystem>
#include <fstream>
#include <future>
#include <unordered_map>

#ifdef linux
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

std::string getHomeDir() {
	const char* homedir;

	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}

	return homedir;
}
#else
// define for her platforms
static_assert();
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

std::string StaticConfig::toString() const {
	Json js{};

	JSON_WRITE(js, root);

	JSON_WRITE(js, window.fullscreen);
	JSON_WRITE(js, window.size.x);
	JSON_WRITE(js, window.size.y);

	return js.dump(1);
}

bool StaticConfig::fromString(const std::string& str) {
	const Json js = [&str]() -> Json {
		try {
			return Json::parse(str);
		} catch (...) {}
		return {};
	}();

	return JSON_READ(js, root) && JSON_READ(js, window.fullscreen) && JSON_READ(js, window.size.x) &&
	       JSON_READ(js, window.size.y);
}

Config::Config(const char** argv, int argc) {
	const auto appPath = fs::path(argv[0]);
	const auto appName = appPath.filename().replace_extension();

	auto args = parseArgs(argv, argc);

	if (args.contains(CONFIG_PATH)) {
		_configPath = args[CONFIG_PATH];
	}
	if (_configPath.empty()) {
		_configPath = getHomeDir();
		_configPath /= appName;
		_configPath /= "config.json";

		if (!fs::exists(_configPath.parent_path()) && !fs::create_directories(_configPath.parent_path())) {
			LERR("Can't create config dir: {}", _configPath.parent_path().string());
		}
	}

	LINFO("Config path: {}", _configPath.string());

	if (_staticConfig.root.empty()) {
		_staticConfig.root = appPath.parent_path().string();
	}

	LINFO("Config: root: {}", _staticConfig.root.string());

	if (!_staticConfig.fromString(readFromFile(_configPath))) {
		LERR("Can't load config: {}", _configPath.string());
		asyncSave();
	}

	if (args.contains(ROOT)) {
		fs::path newRootPath(args[ROOT]);
		if (fs::is_directory(newRootPath)) {
			_staticConfig.root = appPath.parent_path().string();
		} else {
			LERR("Wrong path for root: {}", newRootPath.string());
		}
	}
}

void Config::asyncSave() {
	_asyncOps.remove_if([](auto& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; });

	_asyncOps.push_back(std::async(std::launch::async, [this, staticConfig = _staticConfig]() {
		if (!writeToFile(staticConfig.toString(), _configPath)) {
			LERR("Cant save config: {}", _configPath.string());
		} else {
			LERR("Save config: {}", _configPath.string());
		}
	}));
}