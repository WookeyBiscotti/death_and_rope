#pragma once

#include <fstream>
#include <string>

namespace al {

inline std::string readFromFile(const std::string& filePath) {
	std::ifstream t(filePath);
	if (!t.is_open()) {
		return "";
	}
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	return str;
}

inline bool writeToFile(const std::string& data, const std::string& filePath) {
	std::ofstream t(filePath);
	if (!t.is_open()) {
		return false;
	}

	t.write(data.data(), data.size());

	return !t.bad();
}

}
