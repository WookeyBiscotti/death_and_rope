#pragma once

#include <functional>
#include <string_view>

struct HashOfPtr {
	size_t operator()(const std::string_view& str) const { return std::hash<const char*>()(str.data()); }
};
