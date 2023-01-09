#pragma once

#include <cereal/types/optional.hpp>
#include <optional>

namespace al {

template<class T>
using Optional = std::optional<T>;

}
