#pragma once

#include "alch/engine/system.hpp"
//
#include <deque>
#include <memory>
#include <string>
#include <cassert>

namespace spdlog::sinks {
class sink;
}

namespace {
using SinkImp = spdlog::sinks::sink;
}

namespace al {

class Context;

class Logger: public System {
  public:
	Logger();
	~Logger();

	const std::deque<std::string>& logs() const;

	void exportScriptFunctions(Context& context) override;

  private:
	std::shared_ptr<SinkImp> _sink;
};

}

#if !defined(PROD_BUILD)

#include <spdlog/spdlog.h>

#define LINFO(...)  spdlog::info(__VA_ARGS__)
#define LERR(...)   spdlog::error(__VA_ARGS__)
#define LDEBUG(...) spdlog::debug(__VA_ARGS__)
#define LCRIT(...) spdlog::critical(__VA_ARGS__)
#define LASSERT(...) assert(__VA_ARGS__);

#define LERR_IF(COND, ...)          \
	if (COND) {                     \
		spdlog::error(__VA_ARGS__); \
	}

#else

#define LINFO(...)
#define LERR(...)
#define LDEBUG(...)
#define LCRIT(...)

// condition must executed
#define LERR_IF(COND, ...) \
	{ COND; }

#endif
