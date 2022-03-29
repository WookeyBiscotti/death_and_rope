#pragma once

#pragma once

#include "alch/common/types.hpp"
#include "alch/common/vector2.hpp"
//
#include <nlohmann/json.hpp>

using Json = nlohmann::json;

template<class T>
bool jsonRead(const Json& js, const char* name, T& val) {
	try {
		val = js.at(name);
		return true;
	} catch (...) {}
	return false;
}
template<>
inline bool jsonRead<Vector2u>(const Json& js, const char* name, Vector2u& val) {
	try {
		val.x = js.at(name).at("x");
		val.y = js.at(name).at("y");
		return true;
	} catch (...) {}
	return false;
}
template<>
inline bool jsonRead<Path>(const Json& js, const char* name, Path& val) {
	std::string pathStr;
	if (jsonRead(js, name, pathStr)) {
		val = pathStr;
		return true;
	}
	return false;
}
#define JSON_READ(JS, VAL) jsonRead(JS, #VAL, VAL)

template<class T>
void jsonWrite(Json& js, const char* name, const T& val) {
	js[name] = val;
}
template<>
inline void jsonWrite<Path>(Json& js, const char* name, const Path& val) {
	js[name] = val.string();
}
template<>
inline void jsonWrite<Vector2u>(Json& js, const char* name, const Vector2u& val) {
	js[name]["x"] = val.x;
	js[name]["y"] = val.y;
}
#define JSON_WRITE(JS, VAL) jsonWrite(JS, #VAL, VAL)
