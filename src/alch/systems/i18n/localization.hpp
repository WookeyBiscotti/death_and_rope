#pragma once

#include <utility>

template<class T>
auto tr(T&& arg) {
	return std::forward<T>(arg);
}
