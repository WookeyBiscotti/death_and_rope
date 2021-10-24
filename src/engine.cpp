#include "engine.hpp"

#include "asset_cache.hpp"
#include "context.hpp"
#include <SFML/Graphics.hpp>
#include <logging.hpp>
#include <memory>
#include <thread>
#include <imgui-SFML.h>
#include <imgui.h>

void Engine::run()
{
#if !defined(PROD_BUILD)
    auto sink = std::make_shared<StSink>();
    spdlog::default_logger()->sinks().clear();
    spdlog::default_logger()->sinks().push_back(sink);
    bool isLogShow = true;
#endif

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
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                context.isRuning = false;
            }
            if (event.type == sf::Event::KeyPressed) {
                // LINFO("Key pressed: {}", event.key.code);
            }

            if (currentScene) {
                currentScene->onEvent(event);
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();

        currentScene->onFrame();

#if !defined(PROD_BUILD)
        if (isLogShow) {
            ImGui::Begin("Logs", &isLogShow);
            for (const auto& l : sink->logs) {
                ImGui::TextUnformatted(l.c_str());
            }
            ImGui::End();
        }
#endif

        ImGui::SFML::Render(window);

        if (currentFrame++ == updateFpsEveryFrame) {
            currentFrame = 0;
            text.setString(std::to_string(int(lastFps)));
        }
        auto save = window.getView();
        window.setView(window.getDefaultView());
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