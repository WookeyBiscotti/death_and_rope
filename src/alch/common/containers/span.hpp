#pragma once

#include <absl/types/span.h>

namespace al {

template<class T>
using Span = absl::Span<T>;

template<class... Args>
constexpr auto MakeSpan(Args&&... args) noexcept {
	return absl::MakeSpan(std::forward<Args>(args)...);
}

} // namespace al
