#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/map.hpp"
#include "config_value.hpp"

namespace al {

struct SystemConfigValue {
	enum Access {
		ENGINE,
		USER,
	} access = ENGINE;

	ConfigValue value;

	template<class Archive>
	void serialize(Archive& ar) {
		ar(access, value);
	}
};

using UserConfigValue = ConfigValue;

// class SystemTree {
//   public:
// 	template<class Archive>
// 	void serialize(Archive& ar) {
// 		ar(_values);
// 	}

// 	template<class N, class T>
// 	void set(const N& name, ConfigValue::Access writeAccess, const T& val) {
// 		auto& v = _values[name];
// 		v.value = val;
// 		v.write = writeAccess;
// 	}

// 	template<class N, class T>
// 	T* get(const N& name) {
// 		if (!_values.contains(name)) {
// 			return nullptr;
// 		}
// 		return std::get_if<T>(&_values[name].value);
// 	}

//   private:
// 	Map<String, ConfigValue> _values;
// };

// class UserTree {
//   public:
// 	template<class Archive>
// 	void serialize(Archive& ar) {
// 		ar(_values);
// 	}

// 	template<class N, class T>
// 	void set(const N& name, const T& val) {
// 		auto& v = _values[name];
// 		v.value = val;
// 	}

// 	template<class N, class T>
// 	T* get(const N& name) {
// 		if (!_values.contains(name)) {
// 			return nullptr;
// 		}
// 		return std::get_if<T>(&_values[name]);
// 	}

//   private:
// 	Map<String, ConfigValue> _values;
// };

} // namespace al
