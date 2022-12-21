#pragma once

#include <cereal/types/optional.hpp>
#include <optional>

namespace al {

template<class T>
using Optional = std::optional<T>;

}

template<class Archive, class T>
inline void serialize(Archive& archive, al::Optional<T>& m) {
	archive(m);
}

template<class Archive, class T>
inline void serialize(Archive& archive, const al::Optional<T>& m) {
	archive(m);
}
