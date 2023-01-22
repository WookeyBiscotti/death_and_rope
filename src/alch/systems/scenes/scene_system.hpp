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
	void current(SharedPtr<Scene> scene);

	inline bool onFrame();

	// bool newSceneRequired() const { return _newSceneRequired; }

	// bool findNext(const std::string& name) {
	// 	auto n = find(name);
	// 	auto loaded = !!n;
	// 	_newSceneRequired = true;
	// 	_next = std::move(n);

	// 	return loaded;
	// }

	// void applyNext() {
	// 	if (_current) {
	// 		_current->active(false);
	// 	}
	// 	if (_next) {
	// 		_next->active(true);
	// 	}
	// 	_newSceneRequired = false;
	// 	_current = std::move(_next);
	// }

	void clear() {
		_next = {};
		_newSceneRequired = true;
	}

	std::vector<std::string> list() const;

	void registerScene(const std::string& name, std::function<SharedPtr<Scene>()> creator);

	void removeSceneCache(const std::string& name);

	SharedPtr<Scene> find(const std::string& name);

	void findNext(const std::string& name) { current(find(name)); }

	void exportScriptFunctions(Context& context) override;

  private:
	SharedPtr<Scene> _current;

	bool _newSceneRequired{};
	SharedPtr<Scene> _next;

	struct SceneCreator {
		std::function<SharedPtr<Scene>()> fn;
		SharedPtr<Scene> cache;
	};

	FlatMap<std::string, SceneCreator> _creator;
};

inline bool SceneSystem::onFrame() {
	if (_newSceneRequired) {
		_newSceneRequired = false;
		if (_current) {
			_current->onStop();
		}
		_current = _next;
		if (_current) {
			_current->onStart();
		} else {
			return false;
		}
	}
	if (_current) {
		_current->onFrame();
		return true;
	} else {
		return false;
	}
}

} // namespace al
