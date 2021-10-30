#include "engine.hpp"

#include <context.hpp>
#include <prod_build_utils.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/imgui/imgui_system.hpp>
#include <systems/logging/logger.hpp>
#include <systems/render/render.hpp>
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

	Window window(context);
	context.addSystem(&window);

	ImGuiSystem imgui(context);
	context.addSystem(&imgui);

	Render render(context);
	context.addSystem(&render);

	AssetCache cache(context);
	context.addSystem(&cache);

	context.isRuning = true;

	IF_NOT_PROD_BUILD(bool isLogShow = true);

	auto currentScene = cache.scene("main_menu");
	currentScene->active(true);

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
	while (context.isRuning) {
		const auto t1 = std::chrono::steady_clock::now();
		sf::Event event;
		while (window.window().pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				context.isRuning = false;
			}
			if (event.type == sf::Event::KeyPressed) {
				// LINFO("Key pressed: {}", event.key.code);
			}

			if (currentScene && !ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard) {
				currentScene->onEvent(event);
			}
		}
		ImGui::SFML::Update(window.window(), deltaClock.restart());

		window.window().clear();

		currentScene->onFrame();

#if !defined(PROD_BUILD)
		if (isLogShow) {
			ImGui::Begin("Logs", &isLogShow);
			for (const auto& l : logger.logs()) {
				ImGui::TextUnformatted(l.c_str());
			}
			ImGui::End();
		}
#endif

		ImGui::SFML::Render(window.window());

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

		if (context.nextScene) {
			currentScene->active(false);
			currentScene = std::move(context.nextScene);
			currentScene->active(true);
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