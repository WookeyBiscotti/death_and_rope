#pragma once

template<class Cont, class Element, class Fn>
void ifExist(Cont& cont, Element&& element, Fn&& fn) {
	if (auto found = cont.find(element); found != cont.end()) {
		fn(found->second);
	}
}
