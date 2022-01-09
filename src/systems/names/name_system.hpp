#pragma once

#include <engine/system.hpp>
//
#include <deque>
#include <string>
#include <unordered_map>

class Entity;

class NameSystem: public System {
	friend class Name;

  public:
	Entity* find(const std::string& name) {
		if (auto found = _names.find(name); found != _names.end()) {
			return found->second.back();
		}

		return nullptr;
	}

	void exportScriptFunctions(Context& context) override;

  private:
	size_t add(const std::string& name, Entity* obj) {
		if (!name.empty()) {
			auto& objs = _names[name];
			objs.push_back(obj);

			return objs.size();
		}

		return 0;
	}

	size_t remove(const std::string& name) {
		if (auto found = _names.find(name); found != _names.end()) {
			found->second.pop_back();
			if (found->second.empty()) {
				_names.erase(found);
				return 0;
			} else {
				return found->first.size();
			}
		}

		return {};
	}

  private:
	std::unordered_map<std::string, std::deque<Entity*>> _names;
};