#include "alch/common/containers/inline_vector.hpp"
#include "alch/common/test_framework.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/engine.hpp"
#include "alch/systems/scenes/scene_generic.hpp"
#include "alch/systems/scenes/scene_system.hpp"

#include <thread>

#pragma once

namespace al {

class TestApp {
  public:
	static TestApp& get() {
		static auto s = TestApp();
		return s;
	}

	void exit() {
		std::scoped_lock lk(_guard);
		_isRunning = false;
	}

	void waitExit();

	void push(std::function<void(Context&)> fn);

  private:
	TestApp();

  private:
	std::thread _work;
	std::mutex _guard;
	bool _isRunning;
	std::unique_ptr<al::Context> _context;
	al::InlinedVector<std::function<void(Context&)>, 64> _queue;
};

} // namespace al
