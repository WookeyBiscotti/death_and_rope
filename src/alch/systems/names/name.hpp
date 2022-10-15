#pragma once

#include "alch/common/archive.hpp"
#include "alch/engine/component.hpp"
//
#include <string>

namespace al {

class Entity;

class Name: public Component {
  public:
	Name(Entity& entity, std::string name = "");

	const std::string& name() const { return _name; }
	void name(const std::string& name);

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

	ALCH_COMPONENT_NAME(Name);
  private:
	std::string _name;
};

}
