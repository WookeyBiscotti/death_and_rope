#include "scene_system.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/scripts/scripts.hpp"
//
#include <chaiscript/chaiscript.hpp>

using namespace al;

void SceneSystem::registerScene(const std::string& name, std::function<std::shared_ptr<Scene>()> creator) {
	_creator[name] = {std::move(creator), nullptr};
}

void SceneSystem::removeSceneCache(const std::string& name) {
	if (auto found = _creator.find(name); found != _creator.end()) {
		found->second.cache = nullptr;
	}
}

std::shared_ptr<Scene> SceneSystem::find(const std::string& name) {
	if (auto found = _creator.find(name); found != _creator.end()) {
		LINFO("Scene found: {}", name);
		if (found->second.cache) {
			return found->second.cache;
		} else {
			auto ptr = found->second.fn();
			found->second.cache = ptr;
			return ptr;
		}
	}
	LERR("Scene not found: {}", name);

	return nullptr;
}

std::vector<std::string> SceneSystem::list() const {
	std::vector<std::string> ret;
	for (auto s : _creator) {
		ret.push_back(s.first);
	}
	return ret;
}

void SceneSystem::exportScriptFunctions(Context& context) {
	using namespace chaiscript;
	auto& chai = context.systemRef<Scripts>().internal();
	chai.add(var(this), "scene");
	chai.add(user_type<SceneSystem>(), "SceneSystem");
	chai.add(fun(&SceneSystem::findNext), "findNext");
	chai.add(fun(&SceneSystem::applyNext), "applyNext");
	chai.add(fun(&SceneSystem::list), "list");
	chai.add(fun(&SceneSystem::find), "find");
	chai.add_global_const(const_var(std::shared_ptr<Scene>{}), "nullScene");

	chai.add(fun(&Scene::root), "root");
	chai.add(user_type<Scene>(), "Scene");

	chai.add(fun(&SceneSystem::current), "current");
}
