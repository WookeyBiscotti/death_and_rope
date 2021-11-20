#pragma once

#include <engine/component.hpp>
//
#include <string>

class Entity;

class Name: public Component {
  public:
	Name(Entity& entity, std::string name);

	const std::string& name() const { return _name; }
	void name(const std::string& name);

  private:
	std::string _name;
};