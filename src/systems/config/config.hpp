#pragma once

#include <types.hpp>
#include <vector2.hpp>
//
#include <future>
#include <list>
#include <string>

struct StaticConfig {
	Path root;
	std::string appName;

	struct Window {
		Vector2u size{800, 600};
		bool fullscreen{};
	} window;

	std::string toString() const;
	bool fromString(const std::string& str);
};

class Config {
  public:
	Config(const char** argv, int argc);

	const StaticConfig& staticConfig() const { return _staticConfig; }

	void asyncSave();

  private:
	Path _configPath;

	StaticConfig _staticConfig;

	std::list<std::future<void>> _asyncOps;
};