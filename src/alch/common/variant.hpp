#pragma once

#include <cereal/types/variant.hpp>
#include <variant>

namespace al {

using VariantEmpty = std::monostate;

template<class... Ts>
using Variant = std::variant<Ts...>;

} // namespace al
