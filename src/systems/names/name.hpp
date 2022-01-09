#pragma once

#include <common/archive.hpp>
#include <engine/component.hpp>
//
#include <string>

class Entity;

class Name: public Component {
  public:
	Name(Entity& entity, std::string name = "");

	const std::string& name() const { return _name; }
	void name(const std::string& name);

	template<class Archive>
	void serialize(Archive& ar) {
		if (!_name.empty()) {
			name("");
		}
		ar(_name);
		name(_name);
	}

	void serialize(IArchive& ar) {
		if (!_name.empty()) {
			name("");
		}
		ar(_name);
		name(_name);
	}

	void serialize(OArchive& ar) const { ar(_name); }

  private:
	std::string _name;
};