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

template<class T>
void serialize(const VarOArchive& a, T& o) {
	std::visit([&o](const OBinaryArchive& a) { a(o); }, [&o](const OJSONArchive& a) { a(o); },
	    a);
}
template<class T>
void serialize(VarIArchive& a, const T& o) {
	std::visit([&o](IBinaryArchive& a) { a(o); }, [&o](IJSONArchive& a) { a(o); },
	    a);
}

} // namespace al
