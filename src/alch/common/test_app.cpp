#include "test_app.hpp"

using namespace al;

TestApp::TestApp() {
	auto threadFn = [this]() {
		_isRunning = true;

		EngineConfig config;
		config.enableDefaultScenes = false;
		config.windowName = "Tests";
		config.startScene = "Tests";
		config.localizationFile = "/home/alex/code/game/data/i18n/i18n.json";

		config.preBegin = [&, this](Context& c) {
			c.system<SceneSystem>()->registerScene("Tests", [&c, this]() {
				return std::make_shared<SceneGeneric>(
				    c, [this] {},
				    [&c, this]() {
					    std::unique_lock lk(_guard);
					    if (!_isRunning) {
						    c.systemRef<SceneSystem>().exit();
					    }
					    if (!_queue.empty()) {
						    auto t = std::move(_queue.back());
						    _queue.pop_back();
						    lk.unlock();
						    t(*this->_context.get());
					    }
				    });
			});
		};

		const char* argv = "";
		auto ctx = std::make_unique<Context>();
		auto& e = ctx->createSystem<Engine>();
		e.run(&argv, 1, config);
	};
}
