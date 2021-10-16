#pragma once
#include <string>

class AssetCache;

class Asset
{
public:
    virtual ~Asset() = default;
    Asset(std::string name)
        : _name(std::move(name)){};

    virtual bool load(AssetCache& cache) = 0;

    const auto& name() const { return _name; }

private:
    std::string _name;
};