#pragma once

#include "alch/common/archive.hpp"
#include "alch/engine/component.hpp"
//
#include <string>

class Entity;

class Name: public Component {
  public:
	Name(Entity& entity, std::string name = "");

	const std::string& name() const { return _name; }
	void name(const std::string& name);

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

  private:
	std::string _name;
};
