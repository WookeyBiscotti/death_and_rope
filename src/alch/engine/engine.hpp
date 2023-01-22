#pragma once

#include "alch/common/smart_ptr.hpp"
#include "alch/common/types.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/config/config.hpp"
//
#include <functional>
#include <string>

namespace al {

class Context;
struct EngineConfig {
	std::string windowName;
	Path root;
	Path resourcesRoot;
	Path localizationFile;
	std::string startScene;
	std::function<void(Context& context)> preBegin;
	std::function<void(Context& context)> preEnd;
	bool enableDefaultScenes{};
};

class Scene;

class Engine: public System {
  public:
	Engine(Context& context, ConfigSystem::ConfigTree config, const char** argv, int argc);

	void run(SharedPtr<Scene> scene);

	void exportScriptFunctions(Context& context) override;

  private:
	void registerStdComponents();

  private:
	EngineConfig _config;
};

} // namespace al
