#pragma once
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>


#include "alch/common/archive.hpp"
#include "alch/common/containers/recursive_tree.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/types.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/system.hpp"
#include "alch/systems/config/config_tree.hpp"
#include "alch/systems/logging/logger.hpp"

//
#include <future>
#include <list>
//
#include "alch/common/archive.hpp"

namespace al {

class Context;
class ConfigSystem: public System {
  public:
	using ConfigTree = RecursiveTreeNode<String, SystemConfigValue>;

	ConfigSystem(Context& context, const ConfigTree& startConfig, const char** argv, int argc);

	template<class T>
	const T* value(const char* name) const;
	template<class T>
	void value(const char* name, T&&);

	template<class T>
	const T& valueOr(const char* name, T&& val) const;

	const ConfigValue* valueVariant(const char* name) const;
	ConfigValue* valueVariant(const char* name);

	static ConfigTree createDefaultConfig();

  private:
	void asyncConfigSave();

  private:
	Path _configPath;
	Path _pathToApp;

	std::list<std::future<void>> _asyncOps;

	ConfigTree _systemConfig;
	RecursiveTreeNode<String, UserConfigValue> _userConfig;
};

template<class T>
const T* ConfigSystem::value(const char* name) const {
	auto v = valueVariant(name);
	if (v) {
		return std::get_if<T>(v);
	}

	return nullptr;
}

template<class T>
void ConfigSystem::value(const char* name, T&& val) {
	auto sFound = _systemConfig.find(name);
	if (!sFound) {
		_userConfig[name] = std::move(val);
	} else {
		if (sFound->access == SystemConfigValue::Access::USER) {
			_userConfig[name] = std::move(val);
		} else {
			LWARN("You can't change system config variable: {}", name);
		}
	}
}

template<class T>
const T& ConfigSystem::valueOr(const char* name, T&& val) const {
	auto v = value<T>(name);
	if (v) {
		return *v;
	}

	return std::move(val);
}

} // namespace al
