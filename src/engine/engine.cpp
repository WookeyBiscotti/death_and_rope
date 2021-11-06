#include <common/prod_build_utils.hpp>
#include <engine/context.hpp>
#include <engine/engine.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/broker/broker.hpp>
#include <systems/config/config.hpp>
#include <systems/debug/debug_system.hpp>
#include <systems/imgui/imgui_system.hpp>
#include <systems/logging/logger.hpp>
#include <systems/render/render.hpp>
#include <systems/scenes/scene_system.hpp>
#include <systems/window/window.hpp>
//
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
//
#include <memory>
#include <thread>

void Engine::run(const char** argv, int argc) {
	Context context;

	IF_NOT_PROD_BUILD(Logger logger);
	IF_NOT_PROD_BUILD(context.addSystem(&logger));

	Broker broker;
	context.addSystem(&broker);

	Config config(context, argv, argc);
	context.addSystem(&config);

	Window window(context);
	context.addSystem(&window);

	ImGuiSystem imgui(context);
	context.addSystem(&imgui);

	Render render(context);
	context.addSystem(&render);

	AssetCache cache(context);
	context.addSystem(&cache);

	SceneSystem scenes;
	context.addSystem(&scenes);

	DebugSystem debug(context);
	context.addSystem(&debug);

	auto currentScene = cache.scene("main_menu");
	scenes.next(std::move(currentScene));
	scenes.applyNext();

	float lastFps = 60;
	while (scenes.current()) {
		const auto t1 = std::chrono::steady_clock::now();

		broker.send(EngineOnFrameStart{.lastFps = lastFps});

		window.pullEvents();
		imgui.update();

		// 	if (event.type == sf::Event::Closed) {
		// 		context.isRuning = false;
		// 	}
		// 	if (event.type == sf::Event::KeyPressed) {
		// 		// LINFO("Key pressed: {}", event.key.code);
		// 	}

		// 	if (currentScene && !ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard) {
		// 		currentScene->onEvent(event);
		// 	}
		// }

		window.window().clear();

		scenes.current()->onFrame();

		broker.send(EngineOnFramePreRender{});

		imgui.render();
		render.render();

		broker.send(EngineOnFramePostRender{});

		window.window().display();

		if (scenes.next()) {
			scenes.applyNext();
		}

		broker.send(EngineOnFrameEnd{});

		constexpr auto T = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 60;
		const auto t2 = std::chrono::steady_clock::now();
		const auto dt = t2 - t1;
		lastFps = (std::chrono::seconds(1) / dt);
		if (dt < T) {
			const auto delay = (T - dt);
			std::this_thread::sleep_for(delay);
		}
	}
}