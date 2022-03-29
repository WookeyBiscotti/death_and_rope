#pragma once

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

// using OArchive = cereal::JSONOutputArchive;
// using IArchive = cereal::JSONInputArchive;

using OArchive = cereal::BinaryOutputArchive;
using IArchive = cereal::BinaryInputArchive;
