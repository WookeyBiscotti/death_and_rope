#pragma once

#if !defined(PROD_BUILD)

#include <deque>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

template<typename Mutex, size_t N = 1024>
class Sink : public spdlog::sinks::base_sink<Mutex>
{
public:
    Sink() { logs.resize(N); }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
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

#include "spdlog/details/null_mutex.h"
#include <mutex>
using MtSink = Sink<std::mutex>;
using StSink = Sink<spdlog::details::null_mutex>;

#define LINFO(...) spdlog::info(__VA_ARGS__);

#else

#define LINFO(...) ;

#endif