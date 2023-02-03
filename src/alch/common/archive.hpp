#pragma once

#include "variant.hpp"
//
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
//
namespace al {

// using OArchive = cereal::JSONOutputArchive;
// using IArchive = cereal::JSONInputArchive;

using OArchive = cereal::BinaryOutputArchive;
using IArchive = cereal::BinaryInputArchive;

using OBinaryArchive = cereal::BinaryOutputArchive;
using OJSONArchive = cereal::JSONOutputArchive;

using IBinaryArchive = cereal::BinaryInputArchive;
using IJSONArchive = cereal::JSONInputArchive;

using VarOArchive = Variant<OBinaryArchive, OJSONArchive>;
using VarIArchive = Variant<IBinaryArchive, IJSONArchive>;

template<class... Args>
void save(al::VarOArchive& a, Args&&... args) {
	std::visit([&](auto& archive) { archive(std::forward<Args>(args)...); }, a);
}
template<class... Args>
void load(al::VarIArchive& a, Args&&... args) {
	std::visit([&](auto& archive) { archive(std::forward<Args>(args)...); }, a);
}
} // namespace al
