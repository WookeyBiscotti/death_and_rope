#include "filesystem.hpp"

#include <common/file.hpp>
#include <engine/context.hpp>
#include <systems/config/config.hpp>
//
#include <fstream>

FileSystem::FileSystem(Context& context): _context(context){};

void FileSystem::save(const Path& path, const std::string& data) {
	const auto& root = _context.systemRef<Config>().staticConfig().root;
	writeToFile(data, path);
}

std::string FileSystem::load(const Path& path) {
	const auto& root = _context.systemRef<Config>().staticConfig().root;

	return readFromFile(path);
}

std::pair<std::unique_ptr<OArchive>, std::unique_ptr<std::ofstream>> FileSystem::oArchive(const Path& path) {
	std::pair<std::unique_ptr<OArchive>, std::unique_ptr<std::ofstream>> result;
	const auto& root = _context.systemRef<Config>().staticConfig().root;

	result.second = std::make_unique<std::ofstream>(root / path);
	result.first = std::make_unique<OArchive>(*result.second);

	return result;
}

std::pair<std::unique_ptr<IArchive>, std::unique_ptr<std::ifstream>> FileSystem::iArchive(const Path& path) {
	std::pair<std::unique_ptr<IArchive>, std::unique_ptr<std::ifstream>> result;
	const auto& root = _context.systemRef<Config>().staticConfig().root;

	result.second = std::make_unique<std::ifstream>(root / path);
	result.first = std::make_unique<IArchive>(*result.second);

	return result;
}