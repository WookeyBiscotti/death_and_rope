#include "test_app.hpp"

#include <alch/systems/config/config.hpp>
#include <thread>

using namespace al;

TestApp::TestApp() {
	_isRunning = true;

	auto threadFn = [this]() {
		// EngineConfig config;
		// config.enableDefaultScenes = false;
		// config.windowName = "Tests";
		// config.startScene = "Tests";
		// config.localizationFile = "/home/alex/code/game/data/i18n/i18n.json";

		// config.preBegin = [&, this](Context& c) {
		// 	c.system<SceneSystem>()->registerScene("Tests", [&c, this]() {
		// 		return SharedPtr<SceneGeneric>::make(
		// 		    c, [this] {},
		// 		    [&c, this]() {
		// 			    std::unique_lock lk(_guard);
		// 			    if (!_isRunning && _queue.empty()) {
		// 				    c.systemRef<SceneSystem>().clear();
		// 			    }
		// 			    if (!_queue.empty()) {
		// 				    auto t = std::move(_queue.back());
		// 				    _queue.pop_back();
		// 				    lk.unlock();
		// 				    t(*this->_context.get());
		// 			    }
		// 		    });
		// 	});
		// };

		ConfigSystem::ConfigTree config;
		const char* argv = "";
		_context = std::make_unique<Context>();
		auto& e = _context->createSystem<Engine>(config, &argv, 1);
		auto scene = SharedPtr<SceneGeneric>::make(
		    *_context, [this] {},
		    [&c = *_context, this]() {
			    std::unique_lock lk(_guard);
			    if (!_isRunning && _queue.empty()) {
				    c.systemRef<SceneSystem>().clear();
			    }
			    if (!_queue.empty()) {
				    auto t = std::move(_queue.back());
				    _queue.pop_back();
				    lk.unlock();
				    t(*this->_context.get());
			    }
		    });
		e.run(scene);
	};

	_work = std::thread(threadFn);
}

void TestApp::push(std::function<void(Context&)> fn) {
	std::scoped_lock lk(_guard);
	_queue.push_back(std::move(fn));
}

void TestApp::waitExit() {
	_work.join();
}
