#pragma once

#include <string>
#include <types.hpp>

struct StaticConfig {
	Path root;

	// std::string toString() const;
	// void fromString(std::string& str);
};

class Config {
  public:
	Config(const char** argv, int argc);

	const StaticConfig& staticConfig() const { return _staticConfig; }

  private:
	StaticConfig _staticConfig;
};