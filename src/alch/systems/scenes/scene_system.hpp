#pragma once

#include "alch/engine/system.hpp"
#include "alch/systems/scenes/scene.hpp"
//
#include <functional>
#include <memory>

namespace al {

class SceneSystem: public System {
  public:
	SceneSystem() = default;
	SceneSystem(const SceneSystem&) = delete;

	const std::shared_ptr<Scene>& current() { return _current; }

	bool newSceneRequired() const { return _newSceneRequired; }

	bool findNext(const std::string& name) {
		auto n = find(name);
		auto loaded = !!n;
		_newSceneRequired = true;
		_next = std::move(n);

		return loaded;
	}

	void applyNext() {
		if (_current) {
			_current->active(false);
		}
		if (_next) {
			_next->active(true);
		}
		_newSceneRequired = false;
		_current = std::move(_next);
	}

	void exit() {
		_next = {};
		_newSceneRequired = true;
	}

	std::vector<std::string> list() const;

	void registerScene(const std::string& name, std::function<std::shared_ptr<Scene>()> creator);
	void removeSceneCache(const std::string& name);
	std::shared_ptr<Scene> find(const std::string& name);

	void exportScriptFunctions(Context& context) override;

  private:
	std::shared_ptr<Scene> _current;
	bool _newSceneRequired{};
	std::shared_ptr<Scene> _next;

	struct SceneCreator {
		std::function<std::shared_ptr<Scene>()> fn;
		std::shared_ptr<Scene> cache;
	};

	std::unordered_map<std::string, SceneCreator> _creator;
};

}
