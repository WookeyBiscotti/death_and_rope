#include "imgui_system.hpp"

#include <engine/context.hpp>
#include <engine/events.hpp>
#include <systems/window/events.hpp>
#include <systems/window/window.hpp>
//
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

ImGuiSystem::ImGuiSystem(Context& context): Receiver(context.systemRef<Broker>()), _context(context) {
	ImGui::SFML::Init(context.systemRef<Window>().window());

	subscribe<WindowEvent>([this](const WindowEvent& e) { ImGui::SFML::ProcessEvent(e.event); });

	subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart& e) {
		ImGui::SFML::Update(_context.systemRef<Window>().window(), _deltaClock.restart());
	});

	subscribe<EngineOnFramePostRender>(
	    [this](const EngineOnFramePostRender& e) { ImGui::SFML::Render(_context.systemRef<Window>().window()); });
}

bool ImGuiSystem::wantCaptureKeyboard() const {
	return ImGui::GetIO().WantCaptureKeyboard;
}

bool ImGuiSystem::wantCaptureMouse() const {
	return ImGui::GetIO().WantCaptureMouse;
}
