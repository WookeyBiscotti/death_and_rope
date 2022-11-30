#pragma once

#include <filesystem>
#include <functional>

namespace al {

template<class T>

using Ref = std::reference_wrapper<T>;

using Path = std::filesystem::path;

} // namespace al
