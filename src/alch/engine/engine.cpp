#include "alch/engine/engine.hpp"

#include "alch/common/prod_build_utils.hpp"
#include "alch/common/type_id.hpp"
#include "alch/engine/context.hpp"
#include "alch/engine/events.hpp"
#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/config/config.hpp"
#include "alch/systems/config/config_defs.hpp"
#include "alch/systems/debug/debug_system.hpp"
#include "alch/systems/filesystem/filesystem.hpp"
#include "alch/systems/group/group_system.hpp"
#include "alch/systems/i18n/localization.hpp"
#include "alch/systems/imgui/imgui_system.hpp"
#include "alch/systems/logging/logger.hpp"
#include "alch/systems/names/name.hpp"
#include "alch/systems/names/name_system.hpp"
#include "alch/systems/physics/body.hpp"
#include "alch/systems/physics/physics.hpp"
#include "alch/systems/render/circle_component.hpp"
#include "alch/systems/render/drawable.hpp"
#include "alch/systems/render/rect_shape.hpp"
#include "alch/systems/render/render.hpp"
#include "alch/systems/render/sprite_component.hpp"
#include "alch/systems/scenes/scene_system.hpp"
#include "alch/systems/scripts/scripts.hpp"
#include "alch/systems/transform/transform.hpp"
#include "alch/systems/ui/ui_system.hpp"
#include "alch/systems/window/window.hpp"
//
#include "alch/scenes/default_scene.hpp"
#include "alch/scenes/dev_menu.hpp"
#include "alch/scenes/editor.hpp"
#include "alch/scenes/gui_exemple_scene.hpp"
#include "alch/scenes/main_menu.hpp"
#include "alch/scenes/test.hpp"
#include "alch/scenes/test_physics.hpp"
#include "alch/scenes/ui_editor/scene.hpp"
//
#include "box2d/b2_user_settings.h"

#include <SFML/Graphics.hpp>
#include <chaiscript/chaiscript.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
//
#include <memory>
#include <thread>

using namespace al;

Engine::Engine(Context& c, ConfigSystem::ConfigTree config, const char** argv, int argc): System(c) {
	IF_NOT_PROD_BUILD(context().createSystem<Logger>());
	auto& broker = context().systemRef<Broker>();
	context().createSystem<ConfigSystem>(ConfigSystem::ConfigTree{}, argv, argc);
	context().createSystem<FileSystem>();
	context().createSystem<Localization>(_config.localizationFile);
	context().createSystem<Scripts>();
	auto& window = context().createSystem<Window>();
	IF_NOT_PROD_BUILD(context().createSystem<ImGuiSystem>());
	auto& render = context().createSystem<Render>();
	auto& ui = context().createSystem<UISystem>();
	context().createSystem<AssetCache>();
	auto& scenes = context().createSystem<SceneSystem>();
	context().createSystem<Physics>();
	context().createSystem<NameSystem>();
	IF_NOT_PROD_BUILD(context().createSystem<DebugSystem>());

	for (auto s : context().systems()) {
		s->exportScriptFunctions(context());
	}
}

static void addStandardScenes(Context& context) {
	auto& scenes = context.systemRef<SceneSystem>();
	scenes.registerScene("default", [&context] { return SharedPtr<DefaultScene>::make(context); });
	scenes.registerScene("dev_menu", [&context] { return SharedPtr<DevMenu>::make(context); });
	scenes.registerScene("test", [&context] { return SharedPtr<TestScene>::make(context); });
	scenes.registerScene("test_physics", [&context] { return SharedPtr<TestPhysicsScene>::make(context); });
	scenes.registerScene("editor", [&context] { return SharedPtr<EditorScene>::make(context); });
	scenes.registerScene("ui_example", [&context] { return SharedPtr<GuiExempleScene>::make(context); });
	scenes.registerScene("__ui_editor", [&context] { return SharedPtr<GuiExempleScene>::make(context); });
}

void Engine::registerStdComponents() {
	Entity::registerComponent<Transform>(context());
	Entity::registerComponent<Group>(context());
	Entity::registerComponent<Name>(context());

	Entity::registerComponent<Camera>(context());
	Entity::registerComponent<Sprite>(context());
	Entity::registerComponent<CircleShape>(context());
	Entity::registerComponent<RectShape>(context());

	Entity::registerComponent<Body>(context());
	Entity::registerComponent<Collider>(context());
}

void Engine::run(SharedPtr<Scene> scene) {
	registerStdComponents();

	auto& config = context().systemRef<ConfigSystem>();

	if (config.valueOr(config::ENABLE_DEFAULT_SCENES, false)) {
		addStandardScenes(context());
	}

	auto& scenes = context().systemRef<SceneSystem>();
	scenes.current(scene);

	auto& window = context().systemRef<Window>();
	auto& broker = context().systemRef<Broker>();
	auto& render = context().systemRef<Render>();
	auto& ui = context().systemRef<UISystem>();
	float lastFps = 60;
	while (true) {
		const auto t1 = std::chrono::steady_clock::now();
		window.pullEvents();

		broker.send(EngineOnFrameStart{.lastFps = lastFps});

		if (!scenes.onFrame()) {
			break;
		}

		broker.send(EngineOnFramePreRender{});

		render.render();

		ui.render();

		broker.send(EngineOnFramePostRender{});

		window.window().display();
		window.window().clear();

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

	if (_config.preEnd) {
		_config.preEnd(context());
	}
}

void Engine::exportScriptFunctions(Context& context) {
	using namespace chaiscript;
	auto& chai = context.systemRef<Scripts>().internal();
	chai.add(fun([&context] {
		auto& scene = context.systemRef<SceneSystem>();
		scene.clear();
	}),
	    "exit");

	chai.add(user_type<Entity>(), "Entity");
	chai.add(fun(&Entity::get<Name>), "name");
	chai.add(fun([](Entity& entity, const std::string& name) { entity.add<Name>(name); }), "addName");
	chai.add(fun([&context](Entity& entity, const std::string& path) {
		// auto& asset = context.systemRef<AssetCache>();
		// std::ofstream f(asset.entityPath() / path);
		// OArchive ar(f);
		// entity.save(ar);
	}),
	    "save");
	chai.add(fun([&context](Entity& entity, const std::string& path) {
		// auto& asset = context.systemRef<AssetCache>();
		// std::ifstream f(asset.entityPath() / path);
		// IArchive ar(f);
		// entity.deserialize(ar);
	}),
	    "load");

	auto& scripts = context.systemRef<Scripts>();
	auto& lua = context.systemRef<Scripts>().internal2();
	scripts.add_func("exit", [&context] {
		auto& scene = context.systemRef<SceneSystem>();
		scene.clear();
	});
	scripts.add_func("context", [&context] { return &context; });
	lua.new_usertype<Entity>("Entity", sol::constructors<Entity(Context&)>());
}
