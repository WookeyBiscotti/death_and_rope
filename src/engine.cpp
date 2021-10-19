#include "engine.hpp"

#include "asset_cache.hpp"
#include "context.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <imgui-SFML.h>
#include <imgui.h>

void Engine::run()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "Death and rope");
    ImGui::SFML::Init(window);

    AssetCache cache;

    Context context(cache, window);
    cache.setContext(&context);

    context.isRuning = true;

    auto currentScene = cache.scene("main_menu");
    currentScene->active(true);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/BLKCHCRY.TTF")) {
        context.isRuning = false;
    }

    sf::Clock deltaClock;
    float lastFps = 60;
    while (context.isRuning) {
        const auto t1 = std::chrono::steady_clock::now();
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                context.isRuning = false;
            }
            if (currentScene) {
                currentScene->onEvent(event);
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();

        currentScene->onFrame();

        ImGui::SFML::Render(window);

        auto save = window.getView();
        window.setView(window.getDefaultView());

        sf::Text text;
        text.setString(std::to_string(int(lastFps)));
        text.setFont(font);
        text.setCharacterSize(20);

        window.draw(text);
        window.setView(save);

        window.display();

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