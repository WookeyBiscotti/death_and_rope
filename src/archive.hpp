#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

using OArchive = cereal::JSONOutputArchive;
using IArchive = cereal::JSONInputArchive;