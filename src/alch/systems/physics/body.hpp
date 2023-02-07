#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"

class b2Body;

namespace al {

class Body: public Component {
	friend class Collider;

  public:
	AL_OBJECT_REGISTER();

	enum Type {
		STATIC,
		DYNAMIC,
		KINETIC,
		BULLET,
	};

	explicit Body(Entity& entity);
	explicit Body(Entity& entity, Type type);

	~Body();

	void position(Vector2f position);

	b2Body& body() { return *_body; }

	void save(OArchive& ar) const override;
	void load(IArchive& ar) override;

  private:
	b2Body* _body{};
};

} // namespace al
