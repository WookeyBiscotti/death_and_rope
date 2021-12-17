#pragma once

#include <common/types.hpp>
//
#include <functional>
#include <string>

class Context;
struct EngineConfig {
	Path root;
	std::string startScene;
	std::function<void(Context& context)> preBegin;
	std::function<void(Context& context)> preEnd;
};

class Engine {
  public:
	void run(const char** argv, int argc, const EngineConfig& config);

	const EngineConfig& config() const { return _config; }

  private:
	EngineConfig _config;
};