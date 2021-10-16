#pragma once

#include <bitset>
#include <cinttypes>

struct TileInfo
{
    std::size_t visualId;
    std::size_t typeId;
    std::bitset<32> collision;

    template<class Archive>
    void serialize(Archive& ar) const
    {
        ar(visualId, typeId, collision);
    }
};