#include "config.hpp"

#include <systems/logging/logger.hpp>
//
#include <filesystem>
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

static const auto CONFIG_ROOT = "config_root"s;
static const auto ROOT = "root"s;

auto parseArgs(const char** argv, int argc) {
	std::unordered_map<std::string, std::string> res;
	for (int i = 0; i != argc;) {
		auto a = argv[i++];
		if (a == CONFIG_ROOT || a == ROOT) {
			if (i == argc) {
				LERR("Wrong program arg: {}", a);
				break;
			}
			res[a] = argv[i++];
		}
	}

	return res;
}

Config::Config(const char** argv, int argc) {
	const auto appPath = fs::path(argv[0]);
	const auto appName = appPath.filename().replace_extension();

	auto args = parseArgs(argv, argc);

	fs::path configRoot;
	if (args.contains(CONFIG_ROOT)) {
		if (!fs::create_directories(args[CONFIG_ROOT])) {
			LERR("Can't create config dir: {}", configRoot.string());
		} else {
			configRoot = args[CONFIG_ROOT];
		}
	} else {
		configRoot = getHomeDir();
		configRoot += appName;
	}

	LINFO("Config: config root: {}", configRoot.string());

	if (args.contains(ROOT)) {
		fs::path newRootPath(args[ROOT]);
		if (fs::is_directory(newRootPath)) {
			_staticConfig.root = appPath.parent_path().string();
		} else {
			LERR("Wrong path for root: {}", newRootPath.string());
		}
	}

	if (_staticConfig.root.empty()) {
		_staticConfig.root = appPath.parent_path().string();
	}

	if (!_staticConfig.root.ends_with('/')) {
		_staticConfig.root += '/';
	}

	LINFO("Config: root: {}", _staticConfig.root);
}