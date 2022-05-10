#pragma once

#include "alch/common/types.hpp"
#include "alch/engine/system.hpp"
//
#include <functional>
#include <string>

class Context;
struct EngineConfig {
	std::string windowName;
	Path root;
	std::string startScene;
	std::function<void(Context& context)> preBegin;
	std::function<void(Context& context)> preEnd;
	bool enableDefaultScenes{};
};

class Engine: public System {
  public:
	void run(const char** argv, int argc, const EngineConfig& config);

	const EngineConfig& config() const { return _config; }

	void exportScriptFunctions(Context& context) override;

  private:
	EngineConfig _config;
};
