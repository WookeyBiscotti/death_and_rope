#pragma once

#include <engine/system.hpp>
#include <systems/scenes/scene.hpp>
//
#include <functional>
#include <memory>

class SceneSystem: public System {
  public:
	const std::shared_ptr<Scene>& current() { return _current; }

	void next(std::shared_ptr<Scene> scene) { _next = std::move(scene); }
	const std::shared_ptr<Scene>& next() { return _next; }

	bool findNext(const std::string& name) {
		auto n = find(name);
		auto loaded = !!n;
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
		_current = std::move(_next);
	}

	void registerScene(const std::string& name, std::function<std::shared_ptr<Scene>()> creator);
	void removeSceneCache(const std::string& name);
	std::shared_ptr<Scene> find(const std::string& name);

	void exportScriptFunctions(Context& context) override;

  private:
	std::shared_ptr<Scene> _current;
	std::shared_ptr<Scene> _next;

	struct SceneCreator {
		std::function<std::shared_ptr<Scene>()> fn;
		std::shared_ptr<Scene> cache;
	};

	std::unordered_map<std::string, SceneCreator> _creator;
};