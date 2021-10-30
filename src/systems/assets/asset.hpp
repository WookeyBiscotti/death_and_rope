#pragma once
#include <string>

class AssetCache;

class Asset
{
public:
    virtual ~Asset() = default;
    Asset(std::string name)
        : _name(std::move(name)){};

    const auto& name() const { return _name; }
    void name(const std::string& name) { _name = name; }

private:
    std::string _name;
};