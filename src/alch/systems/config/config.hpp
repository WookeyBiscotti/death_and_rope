#pragma once
#include "alch/common/archive.hpp"
#include "alch/common/containers/hash_set.hpp"
#include "alch/common/containers/recursive_tree.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/types.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/config/config_tree.hpp"
#include "alch/systems/logging/logger.hpp"
#include "config_handler.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

//
#include <future>
#include <list>
//
#include "alch/common/archive.hpp"

namespace al {

class Context;
class ConfigSystem: public System {
	friend class ConfigHandler;

  public:
	using ConfigTree = RecursiveTreeNode<String, SystemConfigValue>;

	ConfigSystem(Context& context, const ConfigTree& startConfig, const char** argv, int argc);

	template<class T>
	const T* value(const String& s) const;
	template<class T>
	void value(const String& s, T&&);

	template<class T>
	const T& valueOr(const String& s, T&& val) const;

	const ConfigValue* valueVariant(const String& s) const;
	ConfigValue* valueVariant(const String& s);

	static ConfigTree createDefaultConfig();

  private:
	void asyncConfigSave();

	void subscribe(ConfigHandler& handler, const String& event, std::function<void(const String&)>);

	void unsubscribe(ConfigHandler& handler, const String& event);
	void unsubscribe(ConfigHandler& handler);

  private:
	Path _configPath;
	Path _pathToApp;

	std::list<std::future<void>> _asyncOps;

	ConfigTree _systemConfig;
	RecursiveTreeNode<String, UserConfigValue> _userConfig;

	FlatMap<ConfigHandler*, FlatSet<String>> _handlerToEvent;
	FlatMap<String, FlatMap<ConfigHandler*, std::function<void(const String&)>>> _eventToHandler;
};

template<class T>
const T* ConfigSystem::value(const String& name) const {
	auto v = valueVariant(name);
	if (v) {
		return std::get_if<T>(v);
	}

	return nullptr;
}

template<class T>
void ConfigSystem::value(const String& name, T&& val) {
	auto sFound = _systemConfig.find(name);
	if (!sFound) {
		_userConfig[name] = std::move(val);
	} else {
		if (sFound->access == SystemConfigValue::Access::USER) {
			_userConfig[name] = std::move(val);
		} else {
			LWARN("You can't change system config variable: {}", name);
			return;
		}
	}
	auto found = _eventToHandler.find(name);
	for (const auto& e : found->second) {
		e.second(name);
	}
}

template<class T>
const T& ConfigSystem::valueOr(const String& name, T&& val) const {
	auto v = value<T>(name);
	if (v) {
		return *v;
	}

	return std::move(val);
}

} // namespace al
