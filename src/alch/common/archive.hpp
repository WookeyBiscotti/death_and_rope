#pragma once

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

using OArchive = cereal::JSONOutputArchive;
using IArchive = cereal::JSONInputArchive;
// class OArchive: public cereal::BinaryOutputArchive {};
// class IArchive: public cereal::BinaryInputArchive {};
