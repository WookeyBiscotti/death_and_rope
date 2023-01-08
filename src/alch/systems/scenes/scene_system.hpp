#pragma once

#include "alch/common/smart_ptr.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/scenes/scene.hpp"
//
#include <functional>

namespace al {

class SceneSystem: public System {
  public:
	SceneSystem(Context& context);

	const SharedPtr<Scene>& current() { return _current; }

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

	void registerScene(const std::string& name, std::function<SharedPtr<Scene>()> creator);
	void removeSceneCache(const std::string& name);
	SharedPtr<Scene> find(const std::string& name);

	void exportScriptFunctions(Context& context) override;

  private:
	SharedPtr<Scene> _current;
	bool _newSceneRequired{};
	SharedPtr<Scene> _next;

	struct SceneCreator {
		std::function<SharedPtr<Scene>()> fn;
		SharedPtr<Scene> cache;
	};

	std::unordered_map<std::string, SceneCreator> _creator;
};

} // namespace al
