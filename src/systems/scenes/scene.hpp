#pragma once

class Context;

namespace sf {
struct Event;
}

class Scene {
  public:
	virtual ~Scene() = default;

	explicit Scene(Context& context): _context(context) {}

	virtual void onFrame(){};
	virtual void onEvent(const sf::Event& e){};

	Context& context() { return _context; }

	virtual bool active() const { return true; };
	virtual void active(bool active){};

  private:
	Context& _context;
};