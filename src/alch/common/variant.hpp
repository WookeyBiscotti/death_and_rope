#pragma once

#include <cereal/types/variant.hpp>
#include <variant>

namespace al {

template<class... Ts>
using Variant = std::variant<Ts...>;

}

template<class Archive, class... Ts>
inline void serialize(Archive& archive, al::Variant<Ts...>& m) {
	archive(m);
}

template<class Archive, class... Ts>
inline void serialize(Archive& archive, const al::Variant<Ts...>& m) {
	archive(m);
}
