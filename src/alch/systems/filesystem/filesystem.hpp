#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/types.hpp"
#include "alch/engine/system.hpp"
//
#include <array>
#include <deque>
#include <memory>

namespace al {

class Context;

class FileSystem: public System {
  public:
	explicit FileSystem(Context& context);

	// Save relative root
	void save(const Path& path, const std::string& data);

	std::string load(const Path& path);

	std::pair<std::unique_ptr<OArchive>, std::unique_ptr<std::ofstream>> oArchive(const Path& path);
	std::pair<std::unique_ptr<IArchive>, std::unique_ptr<std::ifstream>> iArchive(const Path& path);

  private:
	Context& _context;
};

}
