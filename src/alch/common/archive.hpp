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

// using OArchive = cereal::BinaryOutputArchive;
// using IArchive = cereal::BinaryInputArchive;

using OBinaryArchive = cereal::BinaryOutputArchive;
using OJSONArchive = cereal::JSONOutputArchive;

using IBinaryArchive = cereal::BinaryInputArchive;
using IJSONArchive = cereal::JSONInputArchive;

struct OArchive {
	Variant<OBinaryArchive, OJSONArchive> a;
	template<class... Args>
	void operator()(Args&&... args) {
		std::visit([&, this](auto& archive) { archive(std::forward<Args>(args)...); }, a);
	}

	OArchive(const OArchive&) = delete;

	struct JsonType {};
	inline static JsonType Json{};

	struct BinType {};
	inline static BinType Bin{};

	template<class... Args>
	OArchive(BinType, Args&&... args): a(std::in_place_type<OJSONArchive>, std::forward<Args>(args)...) {}
	template<class... Args>
	OArchive(JsonType, Args&&... args): a(std::in_place_type<OBinaryArchive>, std::forward<Args>(args)...) {}
};

struct IArchive {
	Variant<IBinaryArchive, IJSONArchive> a;
	template<class... Args>
	void operator()(Args&&... args) {
		std::visit([&, this](auto& archive) { archive(std::forward<Args>(args)...); }, a);
	}

	struct JsonType {};
	inline static JsonType Json{};

	struct BinType {};
	inline static BinType Bin{};

	template<class... Args>
	IArchive(BinType, Args&&... args): a(std::in_place_type<IJSONArchive>, std::forward<Args>(args)...) {}
	template<class... Args>
	IArchive(JsonType, Args&&... args): a(std::in_place_type<IBinaryArchive>, std::forward<Args>(args)...) {}
};

} // namespace al
