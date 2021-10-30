#include "imgui_system.hpp"

#include <context.hpp>
#include <systems/window/events.hpp>
#include <systems/window/window.hpp>
//
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

ImGuiSystem::ImGuiSystem(Context& context): Receiver(context.systemRef<Broker>()), _context(context) {
	ImGui::SFML::Init(context.systemRef<Window>().window());

	subscribe<WindowEvent>([this](const WindowEvent& e) { ImGui::SFML::ProcessEvent(e.event); });
}

void ImGuiSystem::update() {
	ImGui::SFML::Update(_context.systemRef<Window>().window(), _deltaClock.restart());
}

void ImGuiSystem::render() {
	ImGui::SFML::Render(_context.systemRef<Window>().window());
}