#include "logger.hpp"

#if !defined(PROD_BUILD)

#include "alch/engine/context.hpp"
#include "alch/systems/scripts/scripts.hpp"
//
#include <chaiscript/chaiscript.hpp>
#include <spdlog/details/null_mutex.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>
//
#include <deque>
#include <mutex>

using namespace al;

template<typename Mutex, size_t N = 1024>
class Sink: public spdlog::sinks::base_sink<Mutex> {
  public:
	Sink() { logs.resize(N); }

  protected:
	void sink_it_(const spdlog::details::log_msg& msg) override {
		spdlog::memory_buf_t formatted;
		spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
		logs.push_front(fmt::to_string(formatted));
		if (logs.size() == N) {
			logs.pop_back();
		}
	}

	void flush_() override {}

  public:
	std::deque<std::string> logs;
};

using MtSink = Sink<std::mutex>;
using StSink = Sink<spdlog::details::null_mutex>;

Logger::Logger(Context& context): System(context) {
	_sink = std::make_shared<StSink>();
	spdlog::default_logger()->sinks().push_back(_sink);
	spdlog::set_pattern("[%T]%^[%l]%$: %v");
}

Logger::~Logger() {
}

void Logger::exportScriptFunctions(Context& context) {
	auto& s = context.systemRef<Scripts>();
	s.internal().add(chaiscript::fun([](const std::string& s) { LINFO(s); }), "log");
}

const std::deque<std::string>& Logger::logs() const {
	return static_cast<StSink*>(_sink.get())->logs;
}

#endif
