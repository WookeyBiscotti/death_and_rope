#pragma once

#include "type_id.hpp"

#include <vector>

namespace al {

template<class... Args>
std::vector<type_id_t> TypeIds() noexcept {
	std::vector<type_id_t> result;
	(result.push_back(TypeId<Args>()), ...);

	return result;
}

}
