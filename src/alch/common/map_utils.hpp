#pragma once

#include <utility>

namespace al {

template<class Cont, class Element, class Fn>
void ifExist(Cont& cont, Element&& element, Fn&& fn) {
	if (auto found = cont.find(element); found != cont.end()) {
		fn(found->second);
	}
}

template<class Cont, class Element>
[[maybe_unused]] auto findItem(Cont& cont, Element&& element) {
	if (auto found = cont.find(element); found != cont.end()) {
		return std::make_pair(&found->second, found);
	}

	return std::make_pair(decltype(&cont.begin()->second){nullptr}, cont.end());
}

template<class Cont, class Element>
[[maybe_unused]] auto findValue(Cont& cont, Element&& element) {
	if (auto found = cont.find(element); found != cont.end()) {
		return &found->second;
	}

	return decltype(&cont.begin()->second){nullptr};
}

}
