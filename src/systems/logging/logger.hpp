#pragma once

#include <deque>
#include <memory>
#include <string>

namespace spdlog::sinks {
class sink;
}

namespace {
using SinkImp = spdlog::sinks::sink;
}

class Logger {
  public:
	Logger();
	~Logger();

	const std::deque<std::string>& logs() const;

  private:
	std::shared_ptr<SinkImp> _sink;
};

#if !defined(PROD_BUILD)

#include <spdlog/spdlog.h>

#define LINFO(...) spdlog::info(__VA_ARGS__)
#define LERR(...)  spdlog::error(__VA_ARGS__)

#define LERR_IF(COND, ...)          \
	if (COND) {                     \
		spdlog::error(__VA_ARGS__); \
	}

#else

#define LINFO(...)
#define LERR(...)

// condition must executed
#define LERR_IF(COND, ...) \
	{ COND; }

#endif