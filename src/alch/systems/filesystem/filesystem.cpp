#include "filesystem.hpp"

#include "alch/common/file.hpp"
#include "alch/engine/context.hpp"
#include "alch/systems/config/config.hpp"
#include "alch/systems/config/config_defs.hpp"
//
#include <fstream>

using namespace al;
using namespace al::config;

FileSystem::FileSystem(Context& context): System(context){};

void FileSystem::save(const Path& path, const std::string& data) {
	const auto& root = _context.systemRef<ConfigSystem>().valueOr<String>(USER_DATA_PATH, "");
	writeToFile(data, path);
}

std::string FileSystem::load(const Path& path) {
	const auto& root = _context.systemRef<ConfigSystem>().valueOr<String>(USER_DATA_PATH, "");

	return readFromFile(path);
}

std::pair<std::unique_ptr<OArchive>, std::unique_ptr<std::ofstream>> FileSystem::oArchive(const Path& path) {
	std::pair<std::unique_ptr<OArchive>, std::unique_ptr<std::ofstream>> result;
	const auto& root = _context.systemRef<ConfigSystem>().valueOr<String>(USER_DATA_PATH, "");

	result.second = std::make_unique<std::ofstream>(root / path);
	result.first = std::make_unique<OArchive>(OArchive::Json, *result.second);

	return result;
}

std::pair<std::unique_ptr<IArchive>, std::unique_ptr<std::ifstream>> FileSystem::iArchive(const Path& path) {
	std::pair<std::unique_ptr<IArchive>, std::unique_ptr<std::ifstream>> result;
	const auto& root = _context.systemRef<ConfigSystem>().valueOr<String>(USER_DATA_PATH, "");

	result.second = std::make_unique<std::ifstream>(root / path);
	result.first = std::make_unique<IArchive>(IArchive::Json, *result.second);

	return result;
}
