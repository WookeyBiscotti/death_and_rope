#include <common/prod_build_utils.hpp>
#include <engine/context.hpp>
#include <engine/engine.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/broker/broker.hpp>
#include <systems/config/config.hpp>
#include <systems/debug/debug_system.hpp>
#include <systems/group/group_system.hpp>
#include <systems/imgui/imgui_system.hpp>
#include <systems/logging/logger.hpp>
#include <systems/names/name.hpp>
#include <systems/names/name_system.hpp>
#include <systems/physics/physics.hpp>
#include <systems/render/render.hpp>
#include <systems/scenes/scene_system.hpp>
#include <systems/scripts/scripts.hpp>
#include <systems/window/window.hpp>
//
#include <scenes/default_scene.hpp>
#include <scenes/dev_menu.hpp>
#include <scenes/main_menu.hpp>
#include <scenes/test.hpp>
#include <scenes/test_physics.hpp>
#include <scenes/world_editor.hpp>
//
#include <SFML/Graphics.hpp>
#include <chaiscript/chaiscript.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
//
#include <memory>
#include <thread>

static void addStandardScenes(Context& context) {
	auto& scenes = context.systemRef<SceneSystem>();
	scenes.registerScene("default", [&context] { return std::make_shared<DefaultScene>(context); });
	scenes.registerScene("dev_menu", [&context] { return std::make_shared<DevMenu>(context); });
	scenes.registerScene("world_editor", [&context] { return std::make_shared<WorldEditor>(context); });
	scenes.registerScene("test", [&context] { return std::make_shared<TestScene>(context); });
	scenes.registerScene("test_physics", [&context] { return std::make_shared<TestPhysicsScene>(context); });
}

void Engine::run(const char** argv, int argc, const EngineConfig& engineConfig) {
	_config = engineConfig;

	Context context;

	context.addSystem(this);

	IF_NOT_PROD_BUILD(Logger logger);
	IF_NOT_PROD_BUILD(context.addSystem(&logger));

	Broker broker;
	context.addSystem(&broker);

	Config config(context, argv, argc);
	context.addSystem(&config);

	Scripts scripts;
	context.addSystem(&scripts);

	Window window(context);
	context.addSystem(&window);

	IF_NOT_PROD_BUILD(ImGuiSystem imgui(context));
	IF_NOT_PROD_BUILD(context.addSystem(&imgui));

	Render render(context);
	context.addSystem(&render);

	AssetCache cache(context);
	context.addSystem(&cache);

	SceneSystem scenes;
	context.addSystem(&scenes);
	addStandardScenes(context);

	Physics physics(context);
	context.addSystem(&physics);

	NameSystem names;
	context.addSystem(&names);

	IF_NOT_PROD_BUILD(DebugSystem debug(context));
	IF_NOT_PROD_BUILD(context.addSystem(&debug));

	if (_config.preBegin) {
		_config.preBegin(context);
	}

	for (auto s : context.systems()) {
		s->exportScriptFunctions(context);
	}

	scenes.findNext(_config.startScene);
	scenes.applyNext();

	Entity::initSerialization();

	float lastFps = 60;
	while (true) {
		const auto t1 = std::chrono::steady_clock::now();
		window.pullEvents();

		broker.send(EngineOnFrameStart{.lastFps = lastFps});

		if (scenes.current()) {
			scenes.current()->onFrame();
		} else {
			break;
		}

		broker.send(EngineOnFramePreRender{});

		render.render();

		broker.send(EngineOnFramePostRender{});

		window.window().display();
		window.window().clear();

		if (scenes.next()) {
			scenes.applyNext();
		}

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
		_config.preEnd(context);
	}
}

void Engine::exportScriptFunctions(Context& context) {
	using namespace chaiscript;
	auto& chai = context.systemRef<Scripts>().internal();
	chai.add(fun([&context] {
		auto& scene = context.systemRef<SceneSystem>();
		scene.next({});
		scene.applyNext();
	}),
	    "exit");

	chai.add(user_type<Entity>(), "Entity");
	chai.add(fun(&Entity::get<Name>), "name");
	chai.add(fun([](Entity& entity, const std::string& name) { entity.add<Name>(name); }), "addName");
	chai.add(fun([&context](Entity& entity, const std::string& path) {
		auto& asset = context.systemRef<AssetCache>();
		std::ofstream f(asset.entityPath() / path);
		OArchive ar(f);
		entity.serialize(ar);
	}),
	    "save");
	chai.add(fun([&context](Entity& entity, const std::string& path) {
		auto& asset = context.systemRef<AssetCache>();
		std::ifstream f(asset.entityPath() / path);
		IArchive ar(f);
		entity.serialize(ar);
	}),
	    "load");
}