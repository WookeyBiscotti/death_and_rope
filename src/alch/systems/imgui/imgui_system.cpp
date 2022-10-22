#include "imgui_system.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/window/events.hpp"
#include "alch/systems/window/window.hpp"
//
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

using namespace al;

ImGuiSystem::ImGuiSystem(Context& context): System(context) {
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
