#include "name_system.hpp"

#include "name.hpp"

#include "alch/engine/context.hpp"
#include "alch/engine/entity.hpp"
#include "alch/systems/scripts/scripts.hpp"
//
#include <chaiscript/chaiscript.hpp>

using namespace al;

void NameSystem::exportScriptFunctions(Context& context) {
	using namespace chaiscript;
	auto& chai = context.systemRef<Scripts>().internal();

	chai.add(var(this), "name");

	chai.add(fun(&NameSystem::add), "add");
	chai.add(fun(&NameSystem::find), "find");
	chai.add(fun(&NameSystem::remove), "remove");

	chai.add(user_type<Name>(), "Name");
	chai.add(fun<const std::string& (Name::*)() const>(&Name::name), "name");
}
