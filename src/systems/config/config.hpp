#pragma once

#include <common/types.hpp>
#include <common/vector2.hpp>
#include <engine/system.hpp>
#include <systems/broker/sender.hpp>
//
#include <future>
#include <list>
#include <string>

struct StaticConfig {
	Path root;
	std::string appName;

	struct Window {
		Vector2u size{800, 600};
		Vector2u position{0, 0};
		bool fullscreen{};
		bool borderless{};
		bool verticalSync{};
		bool operator==(const Window& other) const = default;
	} window;

	bool operator==(const StaticConfig& other) const = default;

	std::string toString() const;
	bool fromString(const std::string& str);
};

class Context;
class Config: public Sender, public System {
  public:
	Config(Context& context, const char** argv, int argc);

	const StaticConfig& staticConfig() const { return _staticConfig; }
	void staticConfig(const StaticConfig& newConfig);

	void asyncSave();

  private:
	Path _configPath;

	StaticConfig _staticConfig;

	std::list<std::future<void>> _asyncOps;
};