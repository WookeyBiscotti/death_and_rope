#pragma once

#include <optional>

namespace al {

template<class T>
using Opt = std::optional<T>;

}

template<class Archive, class T>
inline void serialize(Archive& archive, al::Opt<T>& m) {
	archive(m.has_value(), m.value_or(T()));
}

template<class Archive, class T>
inline void serialize(Archive& archive, const al::Opt<T>& m) {
	archive(m.has_value(), m.value_or(T()));
}
