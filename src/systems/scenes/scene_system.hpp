#pragma once

#include <systems/scenes/scene.hpp>
//
#include <memory>

class SceneSystem {
  public:
	const std::shared_ptr<Scene>& current() { return _current; }

	void next(std::shared_ptr<Scene> scene) { _next = std::move(scene); }
	const std::shared_ptr<Scene>& next() { return _next; }

	void applyNext() {
		if (_current) {
			_current->active(false);
		}
		if (_next) {
			_next->active(true);
		}
		_current = std::move(_next);
	}

  private:
	std::shared_ptr<Scene> _current;
	std::shared_ptr<Scene> _next;
};