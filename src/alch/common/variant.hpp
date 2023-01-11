#pragma once

#include <cereal/types/variant.hpp>
#include <variant>

namespace al {

struct VariantEmpty {};

template<class... Ts>
using Variant = std::variant<Ts...>;

} // namespace al
