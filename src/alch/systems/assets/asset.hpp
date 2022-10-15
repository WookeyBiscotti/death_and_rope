#pragma once
#include <string>


namespace al {
	
class AssetCache;

class Asset {
  public:
	virtual ~Asset() = default;
	Asset(std::string name): _name(std::move(name)){};

	// path from asset type root
	const auto& name() const { return _name; }
	void name(const std::string& name) { _name = name; }

  private:
	std::string _name;
};

}
