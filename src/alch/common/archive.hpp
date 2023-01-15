#pragma once

#include "variant.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

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

} // namespace al

template<class T>
void serialize(const al::VarOArchive& a, T& o) {
	std::visit([&o](const al::OBinaryArchive& a) { a(o); }, [&o](const al::OJSONArchive& a) { a(o); }, a);
}
template<class T>
void serialize(al::VarIArchive& a, const T& o) {
	std::visit([&o](al::IBinaryArchive& a) { a(o); }, [&o](al::IJSONArchive& a) { a(o); }, a);
}
