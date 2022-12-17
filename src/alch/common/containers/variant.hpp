#pragma once

#include <variant>

namespace al {

template<class... Args>
using Variant = std::variant<Args...>;

}
