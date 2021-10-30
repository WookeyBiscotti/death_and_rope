#include "imgui_system.hpp"

#include <context.hpp>
#include <systems/window/window.hpp>
//
#include <imgui-SFML.h>
#include <imgui.h>

ImGuiSystem::ImGuiSystem(Context& context): _context(context) {
	ImGui::SFML::Init(context.systemRef<Window>().window());
}