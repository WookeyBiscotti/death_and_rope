#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "debug_system.hpp"
#include "systems/window/events.hpp"

#include <engine/context.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/logging/logger.hpp>
#include <systems/window/window.hpp>
//
#include <imgui.h>

DebugSystem::DebugSystem(Context& context): Receiver(context.systemRef<Broker>()), _context(context) {
	_font = context.systemRef<AssetCache>().font("default");

	_text.setFont(_font->sf());
	_text.setCharacterSize(40);
	_text.setScale(0.5, 0.5);

	subscribe<WindowEvent>([this](const WindowEvent& e) {
		if (e.event.type == sf::Event::KeyPressed && e.event.key.code == sf::Keyboard::F12) {
			_isLogShown = !_isLogShown;
		}
	});

	subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart& e) { _lastFps = e.lastFps; });

	subscribe<EngineOnFramePreRender>([this](const EngineOnFramePreRender& e) {
		if (_isLogShown) {
			auto io = ImGui::GetIO();
			const auto W = io.DisplaySize.x;
			const auto H = io.DisplaySize.y;
			ImGui::SetNextWindowSize({W, H / 2});
			ImGui::SetNextWindowPos({0, H / 2});
			ImGui::SetNextWindowBgAlpha(0.5f);
			ImGui::Begin("Logs(F12 to open/close)", nullptr,
			    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
			        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			for (const auto& l : _context.systemRef<Logger>().logs()) {
				ImGui::TextUnformatted(l.c_str());
			}
			ImGui::End();
		}
	});

	subscribe<EngineOnFramePostRender>([this](const EngineOnFramePostRender& e) {
		if (_frameCounter++ == 60) {
			_frameCounter = 0;
			_text.setString(std::to_string(int(_lastFps)));
		}
		auto& window = _context.systemRef<Window>().window();
		auto save = window.getView();
		window.setView(window.getDefaultView());
		window.draw(_text);
		window.setView(save);
	});
}