#include "engine.hpp"

#include <context.hpp>
#include <prod_build_utils.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/broker/broker.hpp>
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

void Engine::run() {
	Context context;

	IF_NOT_PROD_BUILD(Logger logger);
	IF_NOT_PROD_BUILD(context.addSystem(&logger));

	Broker broker;
	context.addSystem(&broker);

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

	context.isRuning = true;

	IF_NOT_PROD_BUILD(bool isLogShow = true);

	auto currentScene = cache.scene("main_menu");
	scenes.next(std::move(currentScene));
	scenes.applyNext();

	sf::Font font;
	if (!font.loadFromFile("assets/fonts/BLKCHCRY.TTF")) {
		context.isRuning = false;
	}

	float lastFps = 60;
	constexpr auto updateFpsEveryFrame = 60;
	auto currentFrame = 0;
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);

	sf::Clock deltaClock;
	while (context.isRuning && scenes.current()) {
		const auto t1 = std::chrono::steady_clock::now();

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

#if !defined(PROD_BUILD)
		if (isLogShow) {
			ImGui::Begin("Logs", &isLogShow);
			for (const auto& l : logger.logs()) {
				ImGui::TextUnformatted(l.c_str());
			}
			ImGui::End();
		}
#endif

		imgui.render();

		if (currentFrame++ == updateFpsEveryFrame) {
			currentFrame = 0;
			text.setString(std::to_string(int(lastFps)));
		}
		auto save = window.window().getView();
		window.window().setView(window.window().getDefaultView());
		window.window().draw(text);
		window.window().setView(save);

		render.render();

		window.window().display();

		if (scenes.next()) {
			scenes.applyNext();
		}

		const auto t2 = std::chrono::steady_clock::now();

		constexpr auto T = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 60;
		const auto dt = t2 - t1;
		lastFps = (std::chrono::seconds(1) / dt);
		if (dt < T) {
			const auto delay = (T - dt);
			std::this_thread::sleep_for(delay);
		}
	}
}