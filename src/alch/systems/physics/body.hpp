#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"

class b2Body;

class Body: public Component {
	friend class Collider;

  public:
	enum Type {
		STATIC,
		DYNAMIC,
		KINETIC,
	};

	explicit Body(Entity& entity);
	explicit Body(Entity& entity, Type type);

	~Body();

	void position(Vector2f position);

	void serialize(OArchive& ar) const override;
	void deserialize(IArchive& ar) override;

  private:
	b2Body* _body{};
};
