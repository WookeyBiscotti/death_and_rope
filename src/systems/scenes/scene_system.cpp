#include "scene_system.hpp"

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
		if (found->second.cache) {
			return found->second.cache;
		} else {
			auto ptr = found->second.fn();
			found->second.cache = ptr;
			return ptr;
		}
	}

	return nullptr;
}