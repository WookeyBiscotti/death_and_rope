#pragma once

namespace al {

class Context;

class System {
  public:
  virtual ~System() = default;
	virtual void exportScriptFunctions(Context& context){};
};

}
